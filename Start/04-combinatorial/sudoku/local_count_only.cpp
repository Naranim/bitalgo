/**

  Zadanie Q: Sudoku.
  Autor: Bartłomiej Szczepanik

  Rozwiązanie wzorcowe z wykorzystaniem jedynie sprawdzania poprawności rozwiązania po każdym uzupełnieniu pola.
  Złożoność rozwiązania: O(9^F * N), gdzie F to liczba wolnych pól na planszy.

**/

#include <cstdio>
#include <vector>

using namespace std;

class Board {
  int kEmptyField;

  bool UsedInASector(int start_row, int start_column, int possible_value) {
    for(int i=0; i<_sector_size; ++i) {
      for(int j=0; j<_sector_size; ++j) {
        if(_board[start_row + i][start_column + j] == possible_value) {
          return true;
        }
      }
    }

    return false;
  }

  bool UsedInAColumn(int column, int possible_value) {
    for(int i=0; i<_board_size; ++i) {
      if(_board[i][column] == possible_value) {
        return true;
      }
    }
    return false;
  }
  bool UsedInARow(int row, int possible_value) {
    for(int i=0; i<_board_size; ++i) {
      if(_board[row][i] == possible_value) {
        return true;
      }
    }
    return false;
  }

public:
  Board(int board_size, int sector_size) : _board_size(board_size), _sector_size(sector_size) {
    kEmptyField = 0;
    _board.resize(_board_size);  
  
    for(int i=0; i<_board_size; ++i) {
      _board[i].resize(_board_size);
    }
  }

  int RowSize() {
    return _board_size;
  }

  int AllFieldsNumber() {
    return _board_size * _board_size;
  }

  int At(int position) {
    return _board[position / _board_size][position % _board_size];
  }

  bool IsEmpty(int position) {
    return _board[position / _board_size][position % _board_size] == kEmptyField;
  }

  void Place(int value, int position) {
    _board[position / _board_size][position % _board_size] = value; 
  }

  void PlaceEmpty(int position) {
    Place(kEmptyField, position);
  }

  bool IsValidToPlace(int possible_value, int position) {
    int row_index = position / _board_size;
    int column_index = position % _board_size;

    return !UsedInARow(row_index, possible_value)
        && !UsedInAColumn(column_index, possible_value)
        && !UsedInASector(row_index - (row_index % _sector_size),
                          column_index - (column_index % _sector_size),
                          possible_value);
  }

private:
  vector<vector<int> > _board;
  int _board_size;
  int _sector_size;
};

bool solve_sudoku(Board* board, int start_from) {
  int position = start_from;
  while(position < board->AllFieldsNumber() && !board->IsEmpty(position)) {
    position++;
  }

  if(position == board->AllFieldsNumber()) {
    return true;
  }

  for(int possible_value = 1; possible_value <= board->RowSize(); ++possible_value) {
    if(!board->IsValidToPlace(possible_value, position)) {
      continue;
    }

    board->Place(possible_value, position);
    bool successful = solve_sudoku(board, start_from + 1);
    if(successful) {
      return true;
    }
    else {
      board->PlaceEmpty(position);
    }
  }

  return false;
}

int main() {
  const int kBoardSize = 9;
  const int kSectorSize = 3;

  Board* board = new Board(kBoardSize, kSectorSize);
  
  for(int position = 0; position < board->AllFieldsNumber(); ++position) {
    int value;
    scanf("%d", &value);

    if(value == 0) {
      board->PlaceEmpty(position);
    }
    else {
      board->Place(value, position);
    }
  }

  solve_sudoku(board, 0);

  for(int position = 0; position < board->AllFieldsNumber(); ++position) {
    if(position > 0 && position % board->RowSize() == 0) {
      printf("\n");
    }

    printf("%d ", board->At(position));
  }

  delete board;
  
  return 0;
}