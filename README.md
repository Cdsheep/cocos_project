# smallgame

一个基于 Cocos2d-x 的小型卡牌原型项目。

当前版本重点不是完整玩法，而是先把 **代码结构** 搭好：

- 数据、逻辑、视图分离
- 支持随机发牌
- 支持顶部牌替换后的“消耗”语义
- 支持连续回退
- 为未来新增卡牌类型、规则类型、回退类型预留扩展点

---

## 目录结构

```text
Classes/
├─ model/   # 纯数据模型
├─ logic/   # 规则、布局计算、发牌、状态变更
└─ view/    # 渲染、点击、动画、按钮
```

### `model/`
负责描述“状态是什么”，不负责处理规则，也不负责显示。

主要文件：

- `CardTypes.h`
  - 定义 `CardSuit`
  - 定义 `CardType`
  - 定义 `CardArea`
  - 定义 `CardData`
- `GameState.h`
  - 定义整局游戏状态 `GameState`
  - 定义动作记录 `MoveRecord`
  - 定义动作类型 `GameActionType`
- `BottomLayoutData.h`
  - 底部区布局参数
- `TableLayoutData.h`
  - 主牌区布局参数

### `logic/`
负责描述“状态怎么变化”。

主要文件：

- `GameController.h/.cpp`
  - 游戏主控制器
  - 负责接收点击后的业务逻辑
  - 负责推进状态、生成回退记录、执行回退
- `GameRules.h`
  - 规则判断
  - 例如：某张牌当前能不能出
- `GameSetupFactory.h/.cpp`
  - 随机发牌
  - 生成初始牌局
- `BottomLayoutCalculator.h/.cpp`
  - 底部区牌的位置计算
- `TableLayoutCalculator.h/.cpp`
  - 主牌区牌的位置计算

### `view/`
负责描述“状态怎么显示出来”。

主要文件：

- `GameBoardLayer.h/.cpp`
  - 主游戏界面
  - 负责背景、卡牌节点、按钮、动画
- `CardNode.h/.cpp`
  - 单张牌的显示节点
- `CardAssetResolver.h/.cpp`
  - 根据牌数据决定该显示哪些资源
- `CardViewAssets.h`
  - 牌面资源描述

---

## 当前核心设计

## 1. 数据层只保留事实，不做推理
例如一张牌的 `CardData` 只说明：

- 它是什么花色和点数
- 它属于什么卡牌类型
- 它当前在哪个区域
- 它当前位置是什么
- 它是不是当前顶部牌

数据层不判断：

- 它能不能出
- 它为什么消失
- 它为什么回退

这些都在逻辑层完成。

---

## 2. 逻辑层负责状态变化
例如：

- 点击一张牌以后是否允许打出
- 顶部牌被新牌替换以后，旧牌进入 `Removed`
- 回退时应该恢复哪两张牌
- 初始牌局如何随机生成

也就是说，**所有规则都应该尽量收敛到 `logic/` 目录中**。

---

## 3. 视图层只根据状态渲染
视图层不负责游戏规则，只做这些事：

- 从 `GameState` 读取牌
- 忽略 `Removed` 区域的牌
- 生成牌节点
- 播放简单动画
- 把点击事件转发给 `GameController`

这样后续即使规则变化，视图层通常不需要大改。

---

## 当前运行流程

### 1. 初始化
`GameBoardLayer::init()` 调用 `GameController::setupGame()`。

### 2. 生成初始局面
`GameController` 调用 `GameSetupFactory::createRandomInitialState()`：

- 生成标准牌堆
- 洗牌
- 分配到底部区、顶部牌、主牌区

### 3. 计算布局
逻辑层调用：

- `TableLayoutCalculator`
- `BottomLayoutCalculator`

把牌的逻辑区域转换成具体位置。

### 4. 视图重建
`GameBoardLayer::rebuildAllCards()` 根据 `GameState` 重新创建牌节点。

### 5. 点击卡牌
`GameBoardLayer` 把点击转给 `GameController::trySelectCard()`，逻辑层决定：

- 是否允许出牌
- 是否生成回退记录
- 是否把旧顶部牌移入 `Removed`

### 6. 点击回退
`GameBoardLayer` 调用 `GameController::undoLastMove()`：

- 逻辑层恢复状态
- 视图层再根据 `MoveRecord` 做反向 `MoveTo`

---

## 区域语义
当前 `CardArea` 有这些值：

- `Table`：主牌区
- `Hand`：手牌区
- `Top`：当前顶部牌
- `Removed`：已经被消耗、当前不再显示

这套设计的好处是：

- “旧顶部牌消失”不需要视图硬编码隐藏
- 只要把状态改成 `Removed`，视图自然不显示
- 回退时只要恢复到原区域即可

---

## 回退系统设计
当前回退不是“简单记一步移动”，而是记录一个完整动作：

- 谁移动到了顶部
- 它原来在哪
- 被消耗的旧顶部牌是谁
- 旧顶部牌原来的顶部位置是什么

这样视图层才能在回退时：

- 让新顶部牌反向移动回原位置
- 让旧顶部牌重新出现并移动回顶部位置

这也是以后扩展新回退类型的基础。

---

这个项目的扩展原则是先在 model 层增加新状态或新类型定义，再在 logic 层补充对应的规则、初始化和回退记录处理，最后让 view 层只根据最新状态决定如何显示和播放动画；也就是说，新增一种卡牌时，先扩 CardType/卡牌定义，再补 GameRules 和发牌逻辑，最后补资源与表现；新增一种回退功能时，先扩 GameActionType 和 MoveRecord，再在控制器里生成和恢复该动作，视图层只消费回退记录做动画，而不直接参与业务判断。
## 如何新增一种卡牌
这部分是后续扩展的重点。

假设未来要新增一种特殊牌，比如：

- Joker
- WildCard
- BombCard
- SkipCard

推荐按下面顺序扩展。

### 第一步：扩展数据模型
在 `CardTypes.h` 的 `CardType` 里新增类型：

```cpp
enum class CardType
{
    Standard,
    Joker
};
```

如果新牌还需要额外属性，建议不要把所有字段直接塞进 `CardData`。

更推荐两种做法：

#### 做法 A：增加少量通用字段
例如：

- `int power`
- `bool canIgnoreRankRule`
- `bool removableAfterPlay`

#### 做法 B：增加独立配置表
例如未来可以做：

- `CardDefinition`
- `CardBehavior`
- `CardRuleProfile`

这样卡牌实例 `CardData` 只保存 `type`，
真正的特殊行为去查定义表。

### 第二步：扩展初始化工厂
在 `GameSetupFactory.cpp` 中决定：

- 新类型的牌是否进入牌堆
- 进入多少张
- 是否随机分配

例如你以后要加入 2 张 Joker，就在生成牌堆时补进去。

### 第三步：扩展规则层
在 `GameRules.h` 中增加新牌逻辑。

例如：

- Joker 可以接任意牌
- BombCard 打出后清空一组牌
- SkipCard 打出后触发特殊效果

重点是：

> **规则扩展优先写在 `GameRules` 或未来独立规则模块里，不要直接写进视图层。**

### 第四步：扩展显示层
如果新卡牌需要特殊外观：

- 在 `CardAssetResolver` 中根据 `CardType` 选择不同资源
- 必要时扩展 `CardNode` 的表现形式

这一步只负责“看起来是什么样”，不负责“规则是什么”。

---

## 如何新增一种回退功能

目前扩展入口是：

- `GameActionType`
- `MoveRecord`
- `GameController::undoLastMove()`

可以用这三个典型例子理解：

### 1. 补牌回退
如果以后有“从牌堆补一张到手牌区”，就新增例如 `DrawHandCard` 这样的动作类型，并记录：

- 补进来的牌 id
- 补牌前的手牌顺序或状态
- 补牌后的目标位置

回退时再把这张牌移除，并恢复补牌前的手牌状态。

### 2. 清除 / 消耗回退
如果以后有“一次性消耗一张牌或清空一组牌”，就新增例如 `ClearTableGroup` 或 `ConsumeCards` 这样的动作类型，并记录：

- 被移除的牌 id 列表
- 它们原来的区域
- 它们原来的位置和顺序

回退时再把这些牌恢复到原区域和原位置。

### 3. 洗牌 / 重排回退
如果以后支持“重新洗手牌”或“重排主牌区”，就新增例如 `ShuffleHandCards` 或 `ReorderTableCards` 这样的动作类型，并记录：

- 重排前顺序
- 重排后顺序
- 相关牌组或牌 id 列表

回退时再恢复到重排前顺序，并重新计算布局。

### 通用扩展步骤
1. 在 `GameActionType` 中新增动作类型。
2. 在 `MoveRecord` 中补充该动作回退所需的数据。
3. 在控制器中新增对应动作入口，并在动作执行后写入 `history`。
4. 在 `undoLastMove()` 中按 `actionType` 分发恢复逻辑。
5. 视图层只根据回退记录播放恢复动画，不直接处理业务规则。

当动作种类继续增多时，建议把统一的 `MoveRecord` 进一步拆成按动作区分的记录结构。

---

## 总结
当前项目的结构目标不是“把 demo 写通”，而是：

- 保持数据、逻辑、视图分离
- 让随机发牌、规则变化、回退恢复彼此解耦
- 为未来新增卡牌、动作和回退能力预留扩展点

