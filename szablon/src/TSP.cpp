#include "TSP.hpp"

#include <algorithm>
#include <stack>
#include <optional>

std::ostream& operator<<(std::ostream& os, const CostMatrix& cm) {
    for (std::size_t r = 0; r < cm.size(); ++r) {
        for (std::size_t c = 0; c < cm.size(); ++c) {
            const auto& elem = cm[r][c];
            os << (is_inf(elem) ? "INF" : std::to_string(elem)) << " ";
        }
        os << "\n";
    }
    os << std::endl;

    return os;
}
// stworzyłem funkcję transpose
void transposeMatrix(cost_matrix_t& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    cost_matrix_t transposed(cols, std::vector<cost_t>(rows));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            transposed[j][i] = matrix[i][j];
        }
    }

    // Update the original matrix with the transposed matrix
    matrix = transposed;
}

/* PART 1 */

/**
 * Create path from unsorted path and last 2x2 cost matrix.
 * @return The vector of consecutive vertex.
 */
path_t StageState::get_path() {
    NewVertex vert = choose_new_vertex();
    append_to_path(vert.coordinates);
    update_cost_matrix(vert.coordinates);
    path_t sorted_path;

    for (std::size_t i = 0; i < matrix_.size(); i++){
        for (std::size_t j = 0; j < matrix_.size(); j++){
            if (matrix_[i][j] != INF)
                append_to_path(vertex_t(i,j));
        }
    }
    //    sorted_path.reserve(unsorted_path_.size()); nie jestem pewien czy to jest konieczne
    // to jest bardzo fajne użycie, wektor o wielkości nieposortowanego, ustawiamy wszystko na false
    std::vector<bool> visited(unsorted_path_.size(), false);
    sorted_path.push_back(unsorted_path_[0].col);
    visited[0] = true;

    for (size_t i = 0; i < unsorted_path_.size(); i++) {
        for (size_t j = 0; j < unsorted_path_.size(); j++) {
            if (visited[j]== false && unsorted_path_[j].row == sorted_path.back()) {
                visited[j] = true;
                sorted_path.push_back(unsorted_path_[j].col);
                break;
            }
        }
    }
    return sorted_path;
}

/**
 * Get minimum values from each row and returns them.
 * @return Vector of minimum values in row.
 */
std::vector<cost_t> CostMatrix::get_min_values_in_rows() const {
    // tworzę pusty wektor najkrótszych dróg w rzędach (czyli ma w sobie dane typu cost_t)
    std::vector<cost_t> min_values (size());
    for (std::size_t i = 0 ; i < size(); i++) //dla kzdego wiersza
    {
        cost_t min_value = INF;
        for (std::size_t j = 0; j < size(); j++)
        {
            cost_t current_value = matrix_[i][j];

            if ( current_value < min_value)
                min_value = current_value;
        }
        if (min_value == INF)
            min_value = 0;
        min_values[i] = min_value;
    }
    return min_values;
}
/** pierwsza wersja */
//std::vector<cost_t> CostMatrix::get_min_values_in_rows() const {
//    // tworzę pusty wektor najkrótszych dróg w rzędach (czyli ma w sobie dane typu cost_t)
//    std::vector<cost_t> min_values = {};
//    for (const auto &row: matrix_){
//        cost_t min_value = INF;
//
//        for(const auto &current_value: row){
//
//            if(current_value < min_value){
//                min_value = current_value;
//            }
//        }
//        // dodaję do wektora minimalnych wartości w rzędach
//        min_values.push_back(min_value);
//    }
//    return min_values;
//}


/**
 * Reduce rows so that in each row at least one zero value is present.
 * @return Sum of values reduced in rows.
 */
cost_t CostMatrix::reduce_rows() {
    std::vector<cost_t> minValuesVector = get_min_values_in_rows();
    cost_t sum = 0;

    for (int i = 0; i < matrix_.size(); i++) {
        for (int j = 0; j < matrix_[0].size(); j++) {
            // teraz sprawdzam każdy element macierzy po kolei, sprawdzam czy nie jest nieskończonością
            if (matrix_[i][j] < INF) {
                matrix_[i][j] -= minValuesVector[i];
            }
        }
        sum += minValuesVector[i];
    }
    return sum;
}

/**
 * Get minimum values from each column and returns them.
 * @return Vector of minimum values in columns.
 */


    std::vector<cost_t> CostMatrix::get_min_values_in_cols() const {
    // tworzę kopię macierzy, którą stransponuję i na niej użyję min_values_in_rows()
    CostMatrix CostMatrix_copy = *this;

    // w razie czego, jakby wcześniej nie została wywołana, nic się nie stanie jak będzie drugi raz bo będą odejmowane zera
    CostMatrix_copy.reduce_rows();
// zrobiłem publiczną funkcję transposeMatrix
//    int rows = CostMatrix_copy.size();
//    int cols = CostMatrix_copy[0].size();
//
//    cost_matrix_t transposed(cols, std::vector<cost_t>(rows));
//
//    for (int i = 0; i < rows; ++i) {
//        for (int j = 0; j < cols; ++j) {
//            transposed[j][i] = CostMatrix_copy[i][j];
//        }
//    }
//    CostMatrix_copy = transposed;
    transposeMatrix(CostMatrix_copy.matrix_);
    return CostMatrix_copy.get_min_values_in_rows();
}

/**
 * Reduces rows so that in each column at least one zero value is present.
 * @return Sum of values reduced in columns.
 */
cost_t CostMatrix::reduce_cols() {
//     chciałem z transponowaniem, reduce_rows i znowu tranbsponować ale w tamtej funkcji wołamy min_values_in_row, dałoby się to zrobic tak żeby funkcja reduce_rows wiedziała że została wywołana przezz reduce_cols()?
    std::vector<cost_t> minValuesVector = get_min_values_in_cols();
    cost_t sum = 0;
    transposeMatrix(matrix_);
    for (int i = 0; i < matrix_.size(); i++) {
        for (int j = 0; j < matrix_[0].size(); j++) {
            // teraz sprawdzam każdy element macierzy po kolei, sprawdzam czy nie jest nieskończonością
            if (matrix_[i][j] < INF) {
                matrix_[i][j] -= minValuesVector[i];
            }
        }
        sum += minValuesVector[i];
    }
    transposeMatrix(matrix_);
    return sum;
}

/**
 * Get the cost of not visiting the vertex_t (@see: get_new_vertex())
 * @param row
 * @param col
 * @return The sum of minimal values in row and col, excluding the intersection value.
 */
cost_t CostMatrix::get_vertex_cost(std::size_t row, std::size_t col) const {
    cost_t minRowValue = INF;
    cost_t minColValue = INF;
    int sum;
    for (int j = 0; j < matrix_[0].size(); j++) {
        if (j != col) { // bo nie chcę na przecięciu
            if (matrix_[row][j] < minRowValue) {
                minRowValue = matrix_[row][j];
            }
        }
    }
    for (int i = 0; i < matrix_.size(); i++) {
        if (i != row) {
            if (matrix_[i][col] < minColValue) {
                minColValue = matrix_[i][col];
            }
        }
    }
    sum = minColValue + minRowValue;
    return sum;
}
/////////////////////////////////////////////////////////////////////////////////////
/** PART 2 */

/**
 * Choose next vertex to visit:
 * - Look for vertex_t (pair row and column) with value 0 in the current cost matrix.
 * - Get the vertex_t cost (calls get_vertex_cost()).
 * - Choose the vertex_t with maximum cost and returns it.
 * @param cm
 * @return The coordinates of the next vertex.
 */
NewVertex StageState::choose_new_vertex() {
//    struct NewVertex {
//        NewVertex(vertex_t v = {}, cost_t cost = 0) : coordinates(v), cost(cost) {}
//
//        vertex_t coordinates;
//        cost_t cost;
//    };
    cost_t max_val = 0;
    vertex_t vert;
    std::size_t m_size = matrix_.size();

    for (std::size_t i = 0; i < m_size; i++) {
        for (std::size_t j = 0; j < m_size; j++) {
            cost_t current_value = matrix_.get_vertex_cost(i, j);
            if(matrix_[i][j] == 0){
                if (current_value > max_val) {
                    max_val = current_value;
                    vert.row = i;
                    vert.col = j;
                }
            }
        }
    }
    return NewVertex(vert, max_val);
}


/**
 * Update the cost matrix with the new vertex.
 * @param new_vertex
 */
void StageState::update_cost_matrix(vertex_t new_vertex) {
//chodzi o to zeby usunąć kolumnę i rząd tego new vertex
    std::size_t m_size = matrix_.size();

    for (std::size_t i = 0; i < m_size; i++) {
        for (std::size_t j = 0; j < m_size; j++) {
            if (i == new_vertex.row || j == new_vertex.col) {
                matrix_[i][j] = INF;
            }
        }
    }
    matrix_[new_vertex.col][new_vertex.row] = INF;
}

/**
 * Reduce the cost matrix.
 * @return The sum of reduced values.
 */
cost_t StageState::reduce_cost_matrix() {
    return matrix_.reduce_cols() + matrix_.reduce_rows();
}

/**
 * Given the optimal path, return the optimal cost.
 * @param optimal_path
 * @param m
 * @return Cost of the path.
 */
 // to już było zrobione, nie ruszac
cost_t get_optimal_cost(const path_t& optimal_path, const cost_matrix_t& m) {
    cost_t cost = 0;

    for (std::size_t idx = 1; idx < optimal_path.size(); ++idx) {
        cost += m[optimal_path[idx - 1]][optimal_path[idx]];
    }

    // Add the cost of returning from the last city to the initial one.
    cost += m[optimal_path[optimal_path.size() - 1]][optimal_path[0]];

    return cost;
}

/**
 * Create the right branch matrix with the chosen vertex forbidden and the new lower bound.
 * @param m
 * @param v
 * @param lb
 * @return New branch.
 */
 // też zrobione
StageState create_right_branch_matrix(cost_matrix_t m, vertex_t v, cost_t lb) {
    CostMatrix cm(m);
    cm[v.row][v.col] = INF;
    return StageState(cm, {}, lb);
}

/**
 * Retain only optimal ones (from all possible ones).
 * @param solutions
 * @return Vector of optimal solutions.
 */
 // też zrobione
tsp_solutions_t filter_solutions(tsp_solutions_t solutions) {
    cost_t optimal_cost = INF;
    for (const auto& s : solutions) {
        optimal_cost = (s.lower_bound < optimal_cost) ? s.lower_bound : optimal_cost;
    }

    tsp_solutions_t optimal_solutions;
    std::copy_if(solutions.begin(), solutions.end(),
                 std::back_inserter(optimal_solutions),
                 [&optimal_cost](const tsp_solution_t& s) { return s.lower_bound == optimal_cost; }
    );

    return optimal_solutions;
}

/**
 * Solve the TSP.
 * @param cm The cost matrix.
 * @return A list of optimal solutions.
 */
tsp_solutions_t solve_tsp(const cost_matrix_t& cm) {

    StageState left_branch(cm);

    // The branch & bound tree.
    std::stack<StageState> tree_lifo;

    // The number of levels determines the number of steps before obtaining
    // a 2x2 matrix.
    std::size_t n_levels = cm.size() - 2;

    tree_lifo.push(left_branch);   // Use the first cost matrix as the root.

    cost_t best_lb = INF;
    tsp_solutions_t solutions;

    while (!tree_lifo.empty()) {

        left_branch = tree_lifo.top();
        tree_lifo.pop();

        while (left_branch.get_level() != n_levels && left_branch.get_lower_bound() <= best_lb) {
            // Repeat until a 2x2 matrix is obtained or the lower bound is too high...

            if (left_branch.get_level() == 0) {
                left_branch.reset_lower_bound();
            }
            /**tu trzeba zrobić*/
            // 1. Reduce the matrix in rows and columns.
            cost_t new_cost = 0; // @TODO (KROK 1)
            new_cost = left_branch.reduce_cost_matrix();
            // 2. Update the lower bound and check the break condition.
            left_branch.update_lower_bound(new_cost);
            if (left_branch.get_lower_bound() > best_lb) {
                break;
            }

            /**tu trzeba zrobić*/
            // 3. Get new vertex and the cost of not choosing it.
            NewVertex new_vertex = NewVertex(); // @TODO (KROK 2)
            new_vertex = left_branch.choose_new_vertex();

            // 4. @TODO Update the path - use append_to_path method.
            left_branch.append_to_path(new_vertex.coordinates);

            // 5. @TODO (KROK 3) Update the cost matrix of the left branch.
            left_branch.update_cost_matrix(new_vertex.coordinates);

            // 6. Update the right branch and push it to the LIFO.
            cost_t new_lower_bound = left_branch.get_lower_bound() + new_vertex.cost;
            tree_lifo.push(create_right_branch_matrix(cm, new_vertex.coordinates,
                                                      new_lower_bound));
        }

        if (left_branch.get_lower_bound() <= best_lb) {
            // If the new solution is at least as good as the previous one,
            // save its lower bound and its path.
            best_lb = left_branch.get_lower_bound();
            path_t new_path = left_branch.get_path();
            solutions.push_back({get_optimal_cost(new_path, cm), new_path});
        }
    }
    return filter_solutions(solutions); // Filter solutions to find only optimal ones.
}
