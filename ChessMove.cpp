/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessMove.hpp"

#include <cassert>
#include "Log.hpp"

bool ChessMove::isMovePossible(ChessBoard::ptr to)
{
	assert(to!=nullptr);

	ChessBoard::BoardPosition_t *king = nullptr;
	auto & width = ChessBoard::param.width;
	auto & height = ChessBoard::param.height;
	if(to->turn==ChessPlayerColour::BLACK)
	{
		// if it's white to move, we are looking for a black king
		king = to->blackKingPos;
	}
	else
	{
		king = to->whiteKingPos;
	}
	
	auto & possiblePieces = ChessBoard::ChessBoard::param.possiblePieces;
	for(
		auto attackingPiece = possiblePieces.begin(), end=possiblePieces.end();
		attackingPiece != end;
		++attackingPiece )
	{
		if(*attackingPiece == EMPTY_CELL || getColour(*attackingPiece) == to->turn)
		{
			continue;
		}
		const MoveTemplate* takeMove = moveParameters.at(*attackingPiece)->takeMove;
		for(auto dir=takeMove->begin(); dir!=takeMove->end(); ++dir)
		{
			for(auto pos=dir->begin(); pos!=dir->end(); ++pos)
			{
				ChessBoard::BoardPosition_t file = (int)king[1] - (int)pos->first;
				ChessBoard::BoardPosition_t rank = (int)king[2] - (int)pos->second;
				if(rank >= height || file >= width )
				{
					break;
				}
				auto piece = to->getPiecePos(file, rank);
				if(piece==*attackingPiece)
				{
					return false;
				}

				if(piece!=EMPTY_CELL)
				{
					break;
				}
			}
		}
	}
	
	return true;
}
std::string ChessMove::getNotation(ChessBoard::ptr from, ChessBoard::ptr to)
{
	std::string result ="";
	
	if(from)
	{
		ChessPiece piece;
		int rankFrom, rankTo;
		char fileFrom, fileTo;
		for(auto it=to->begin(), end=to->end(); it!=end; ++it)
		{
			auto pos = it.getPos();
			if(*it == from->getPiecePos(pos))
			{
				continue;
			}
			
			if(*it==EMPTY_CELL)
			{
				fileFrom = it.getFile();
				rankFrom = it.getRank();
				piece = from->getPiecePos(pos);
			}
			else
			{
				fileTo = it.getFile();
				rankTo = it.getRank();
			}
		}
		
		result = chessPieceStrings[piece] + " " +
			(char)fileFrom + std::to_string(rankFrom) + " " +
			(char)fileTo + std::to_string(rankTo);
	}
	return result;
}

void ChessMove::moveAttempts(
	const ChessMoveRecordingFunction &recFunTake,
	const ChessMoveRecordingFunction &recFunDefend,
	const ChessBoard &cb, const ChessBoard::BoardPosition_t pos,
	const MoveTemplate& mt,
	bool canTake, bool canMoveToEmpty)
{
	const ChessBoard::BoardPosition_t & width = ChessBoard::param.width;
	
	const ChessBoard::BoardPosition_t rank = pos / width;
	const ChessBoard::BoardPosition_t file = pos % width;
	
	ChessBoard::BoardPosition_t newPos;
	
	//Log::info("starting to precess directions");
	for(auto direction = mt.begin(), directionEnd=mt.end(); direction != directionEnd; ++direction)
	{
		//Log::info("starting to process attempts");
		for(auto attempt = direction->begin(), attemptEnd=direction->end(); attempt != attemptEnd; ++attempt)
		{
			const int& rankShift = attempt->second;
			const int& fileShift = attempt->first;
			
			ChessBoard::BoardPosition_t newFile = (int)file + fileShift;
			ChessBoard::BoardPosition_t newRank = (int)rank + rankShift;
			
			if( newFile >= width )
			{
				break;
			}
			
			newPos = cb.getPos(newFile, newRank);
			
			if( newPos >= ChessBoard::param.cellCount )
			{
				break;
			}
			
			if(!cb.isEmptyPos(newPos))
			{
				//Log::info("not isEmptyPos(newPos)");
				if(canTake)
				{
					//Log::info("canTake");
					if(getColour(cb.getPiecePos(newPos)) != getColour(cb.getPiecePos(pos)))
					{
						recFunTake(pos, newPos);
					}
					recFunDefend(pos, newPos);
				}
				break; // stop if a cell isn't empty
			}
			else // if empty
			{
				//Log::info("isEmptyPos(newPos)");
				if(canMoveToEmpty)
				{
					//Log::info("canMoveToEmpty");
					recFunTake(pos, newPos);
				}
				recFunDefend(pos, newPos);
			}
		}
	}
}

std::string ChessMove::generateCompleteMoveChain(ChessBoard::ptr finalBoard)
{
	if(finalBoard==nullptr)
	{
		return std::string("");
	}
	
	auto from = finalBoard->from;
	if(from)
	{
		return std::string("");
	}
	
	return
		generateCompleteMoveChain(from) + 
		std::to_string(finalBoard->moveNum) + std::string(" ") +
		getNotation(from, finalBoard) + std::string(" | ");
}