#include <iostream>
#include "TSP.hpp"

int main() {
    cost_matrix_t cost_matrix = {{INF, 10, 8, 19, 12},
                        {10, INF, 20, 6, 3},
                        {8, 20, INF, 4, 2},
                        {19, 6, 4, INF, 7},
                        {12, 3, 2, 7, INF}};
//    cost_matrix_t cost_matrix {{INF, 12,   3,  45,   6},
//                      {78, INF,  90,  21,   3},
//                      { 5,  56, INF,  23,  98},
//                      {12,   6,   8, INF,  34},
//                      { 3,  98,   3,   2, INF}};




  tsp_solutions_t solutions = solve_tsp(cost_matrix);

  for (auto elem : solutions) {
      std::cout << elem.lower_bound << " : ";
      for (auto pelem : elem.path) {
          std::cout << pelem << " ";
      }
      std::cout << "\n";
  }

  return 0;

}

//
//    //tworzę instancję klasy
//    CostMatrix costMatrix(cm);
//    std::cout<<"Testuje macierz: " << std::endl;
//    std::cout<<costMatrix;
//    //muszę stworzyć instację klasy StageState
////    StageState stageState(costMatrix, )
////    /**TEST 1*/
////    StageState stageState(costMatrix);
////
////    path_t sortedPath = stageState.get_path();
////
////    std::cout << "Sorted Path: ";
////    for (std::size_t vertex : sortedPath) {
////        std::cout << vertex << " ";
////    }
////    std::cout << std::endl;
//
//    /**TEST 2*/
//    std::vector<cost_t> minValuesInRows = costMatrix.get_min_values_in_rows();
//    std::cout << "Najmniejsze wartosci w rzedach: ";
//    for (cost_t value : minValuesInRows) {
//        std::cout << value << " ";
//    }
//    std::cout << std::endl;
//
//
//    /**TEST 3*/
//    int sum_of_reduced_values_row = costMatrix.reduce_rows();
//    std::cout<<"Suma wartosci zredukowanych: ";
//    std::cout<<sum_of_reduced_values_row<<std::endl;
//    std::cout<<"Macierz wyglada teraz tak: "<<std::endl;
//    std::cout<<costMatrix;
//
//    /**TEST 4*/
//    std::vector<cost_t> minValuesInCols = costMatrix.get_min_values_in_cols();
//    std::cout << "Najmniejsze wartosci w kolumnach: ";
//    for (cost_t value : minValuesInCols) {
//        std::cout << value << " ";
//    }
//    std::cout << std::endl;
//
//    /**TEST 5*/
//    int sum_of_reduced_values_col = costMatrix.reduce_cols();
//    std::cout<<"Suma wartosci zredukowanych: ";
//    std::cout<<sum_of_reduced_values_col<<std::endl;
//    std::cout<<"Macierz wyglada teraz tak: "<<std::endl;
//    std::cout<<costMatrix;
//
//    /**TEST 6*/
//    std::size_t row = 3;
//    std::size_t col = 2;
//
//    cost_t cost = costMatrix.get_vertex_cost(row, col);
//    std::cout<< "kosz na przecieciu " << row << " i " << col << " wynosi: " << cost;
//
//
//    /**PARTES DOS*/
//
//
////    /**TEST 7*/
////    //Szukam wszystkich miejsc ktore maja zera, i znajduje to z najwiekszym kosztem
////    std::cout<<"Szukam wszystkich miejsc ktore maja zera, i znajduje to z najwiekszym kosztem: ";
////    costMatrix.;
//
//
//    /**TEST 8*/
//
//
//    return 0;
//}
