#include "logic/GameSetupFactory.h"
#include <algorithm>
#include <random>

namespace smallgame
{
    namespace
    {
        struct DeckCardDefinition
        {
            CardSuit suit = CardSuit::Spade;
            int rank = 1;
        };

        std::vector<DeckCardDefinition> createStandardDeck()
        {
            std::vector<DeckCardDefinition> deck;
            deck.reserve(52);

            const CardSuit suits[] = {
                CardSuit::Spade,
                CardSuit::Heart,
                CardSuit::Club,
                CardSuit::Diamond
            };

            for (CardSuit suit : suits)
            {
                for (int rank = 1; rank <= 13; ++rank)
                {
                    DeckCardDefinition card;
                    card.suit = suit;
                    card.rank = rank;
                    deck.push_back(card);
                }
            }

            return deck;
        }

        CardData makeCard(int id, const DeckCardDefinition& definition, CardArea area, const cocos2d::Vec2& position, bool isTopCard)
        {
            CardData card;
            card.id = id;
            card.suit = definition.suit;
            card.type = CardType::Standard;
            card.rank = definition.rank;
            card.originalArea = area;
            card.currentArea = area;
            card.originalPosition = position;
            card.currentPosition = position;
            card.isTopCard = isTopCard;
            return card;
        }
    }

    GameState GameSetupFactory::createRandomInitialState(const BottomLayoutData& bottomLayoutData,
                                                         const std::vector<TableFanGroupLayout>& tableGroupLayouts,
                                                         int handCardCount,
                                                         int topCardCount)
    {
        GameState state;
        state.bottomLayoutData = bottomLayoutData;
        state.tableGroupLayouts = tableGroupLayouts;

        std::vector<DeckCardDefinition> deck = createStandardDeck();
        std::random_device rd;
        std::mt19937 rng(rd());
        std::shuffle(deck.begin(), deck.end(), rng);

        int deckIndex = 0;
        int nextCardId = 1;

        for (int i = 0; i < handCardCount; ++i)
        {
            CardData card = makeCard(nextCardId++, deck[deckIndex++], CardArea::Hand, cocos2d::Vec2::ZERO, false);
            state.handCardIds.push_back(card.id);
            state.cards[card.id] = card;
        }

        for (int i = 0; i < topCardCount; ++i)
        {
            CardData card = makeCard(nextCardId++, deck[deckIndex++], CardArea::Top, bottomLayoutData.topCardPosition, i == topCardCount - 1);
            if (card.isTopCard)
            {
                state.currentTopCardId = card.id;
            }
            else
            {
                card.currentArea = CardArea::Removed;
                card.currentPosition = cocos2d::Vec2::ZERO;
            }
            state.cards[card.id] = card;
        }

        for (const TableFanGroupLayout& groupLayout : tableGroupLayouts)
        {
            for (size_t index = 0; index < groupLayout.cardIds.size(); ++index)
            {
                CardData card = makeCard(nextCardId++, deck[deckIndex++], CardArea::Table, cocos2d::Vec2::ZERO, false);
                state.tableCardIds.push_back(card.id);
                state.cards[card.id] = card;
            }
        }

        for (size_t groupIndex = 0; groupIndex < state.tableGroupLayouts.size(); ++groupIndex)
        {
            TableFanGroupLayout& mutableGroup = state.tableGroupLayouts[groupIndex];
            mutableGroup.cardIds.clear();
        }

        size_t tableCardIndex = 0;
        for (size_t groupIndex = 0; groupIndex < state.tableGroupLayouts.size(); ++groupIndex)
        {
            TableFanGroupLayout& mutableGroup = state.tableGroupLayouts[groupIndex];
            size_t groupCardCount = tableGroupLayouts[groupIndex].cardIds.size();
            for (size_t index = 0; index < groupCardCount; ++index)
            {
                mutableGroup.cardIds.push_back(state.tableCardIds[tableCardIndex++]);
            }
        }

        state.nextCardId = nextCardId;
        return state;
    }
}
