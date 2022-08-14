#include "matrix.h"

void print_matrix(matrix_t *A) {
  for (int i = 0; i < A->columns; i++) {
    for (int j = 0; j < A->rows; j++) {
      printf("%f\t", A->matrix[i][j]);
    }
    printf("\n");
  }
}

void fill_matrix(matrix_t *A) {
  for (int i = 0; i < A->columns; i++) {
    for (int j = 0; j < A->rows; j++) {
      scanf("%lf", &A->matrix[i][j]);
    }
  }
}

int check_matrix(matrix_t *A) {
  int result = FAILURE;
  if ((A->columns > 0) && (A->rows > 0)) {
    if (A != NULL) {
      result = SUCCESS;
      for (int i = 0; i < A->columns; i++) {
        if (A->matrix[i] == NULL) {
          result = FAILURE;
        }
      }
    }
  }
  return result;
}

int create_matrix(int rows, int columns, matrix_t *result) {
  int resulting_code = 2;
  if (columns > 0 && rows > 0) {
    result->rows = rows;
    result->columns = columns;
    result->matrix = malloc(columns * sizeof(double *));
    if (result->matrix == NULL) {
      exit(1);
    }
    for (int i = 0; i < columns; i++) {
      result->matrix[i] = calloc(rows, sizeof(double));
      if (result->matrix[i] == NULL) {
        exit(1);
      }
    }
    resulting_code = 0;
  }
  return resulting_code;
}

void remove_matrix(matrix_t *A) {
  for (int i = 0; i < A->columns; i++) {
    free(A->matrix[i]);
  }
  free(A->matrix);
}

int eq_matrix(matrix_t *A, matrix_t *B) {
  int resulting_code = 0;
  if ((A->columns == B->columns) && (A->rows == B->rows)) {
    for (int i = 0; i < A->columns; i++) {
      for (int j = 0; j < A->rows; j++) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 0.0000001) {
          resulting_code = 1;
        }
      }
    }
  } else {
    resulting_code = 2;
  }
  return resulting_code;
}

int sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int resulting_code = 0;
  if (check_matrix(A) * check_matrix(B) == 0) {
    resulting_code = 1;
  } else if ((A->columns == B->columns) && (A->rows == B->rows)) {
    create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < result->columns; i++) {
      for (int j = 0; j < result->rows; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }
  } else {
    resulting_code = 2;
  }
  return resulting_code;
}

int sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int resulting_code = 0;
  if (check_matrix(A) * check_matrix(B) == 0) {
    resulting_code = 1;
  } else if ((A->columns == B->columns) && (A->rows == B->rows)) {
    create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < result->columns; i++) {
      for (int j = 0; j < result->rows; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }
  } else {
    resulting_code = 2;
  }
  return resulting_code;
}

int mult_number(matrix_t *A, double number, matrix_t *result) {
  int resulting_code = 0;
  if (check_matrix(A)) {
    create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < result->columns; i++) {
      for (int j = 0; j < result->rows; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  } else {
    resulting_code = 1;
  }
  return resulting_code;
}

int mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int resulting_code = 0;
  if (check_matrix(A) * check_matrix(B) == 0) {
    resulting_code = 1;
  } else if (A->columns == B->rows) {
    create_matrix(A->rows, B->columns, result);
    for (int i = 0; i < result->columns; i++) {
      for (int j = 0; j < result->rows; j++) {
        result->matrix[i][j] = 0;
        for (int k = 0; k < A->columns; k++) {
          result->matrix[i][j] += (A->matrix[i][k] * B->matrix[k][j]);
        }
      }
    }
  } else {
    resulting_code = 2;
  }
  return resulting_code;
}

int transpose(matrix_t *A, matrix_t *result) {
  int resulting_code = 0;
  if (check_matrix(A)) {
    create_matrix(A->columns, A->rows, result);
    for (int i = 0; i < result->columns; i++) {
      for (int j = 0; j < result->rows; j++) {
        result->matrix[i][j] = A->matrix[j][i];
      }
    }
  } else {
    resulting_code = 1;
  }
  return resulting_code;
}

int calc_complements(matrix_t *A, matrix_t *result) {
  int resulting_code = 0;
  if (check_matrix(A)) {
    if (A->columns == A->rows) {
      create_matrix(A->rows, A->columns, result);
      for (int i = 0; i < A->columns; i++) {
        for (int j = 0; j < A->rows; j++) {
          matrix_t temp;
          create_matrix(A->rows - 1, A->columns - 1, &temp);

          int columns_counter = 0;
          for (int k = 0; k < temp.columns; k++, columns_counter++) {
            if (k == i) {
              columns_counter++;
            }
            int rows_counter = 0;
            for (int l = 0; l < temp.rows; l++, rows_counter++) {
              if (l == j) {
                rows_counter++;
              }
              temp.matrix[k][l] = A->matrix[columns_counter][rows_counter];
            }
          }
          double minor = 0.0;
          determinant(&temp, &minor);
          result->matrix[i][j] = pow(-1.0, i + j) * minor;
          remove_matrix(&temp);
        }
      }
    } else {
      resulting_code = 2;
    }
  } else {
    resulting_code = 1;
  }
  return resulting_code;
}

int determinant(matrix_t *A, double *result) {
  int resulting_code = 0;
  *result = 0.0;
  if (check_matrix(A)) {
    if (A->columns == A->rows) {
      if (A->columns == 1) {
        *result = A->matrix[0][0];
      } else {
        for (int k = 0; k < A->rows; k++) {
          matrix_t temp;
          create_matrix(A->rows - 1, A->columns - 1, &temp);

          for (int i = 0; i < temp.columns; i++) {
            int n = 0;
            for (int j = 0; j < temp.rows; j++, n++) {
              if (j == k) {
                n++;
              }
              temp.matrix[i][j] = A->matrix[i + 1][n];
            }
          }

          double temp_result = 0.0;
          determinant(&temp, &temp_result);
          *result += pow(-1.0, k) * A->matrix[0][k] * temp_result;
          remove_matrix(&temp);
        }
      }
    } else {
      resulting_code = 2;
    }
  } else {
    resulting_code = 1;
  }
  return resulting_code;
}

int inverse_matrix(matrix_t *A, matrix_t *result) {
  int resulting_code = 0;
  if (check_matrix(A)) {
    if (A->columns == A->rows) {
      double det;
      determinant(A, &det);
      if (det != 0) {
        matrix_t A_compliments;
        calc_complements(A, &A_compliments);

        matrix_t A_compliments_transponsed;
        transpose(&A_compliments, &A_compliments_transponsed);

        create_matrix(A->rows, A->columns, result);
        for (int i = 0; i < A->columns; i++) {
          for (int j = 0; j < A->rows; j++) {
            result->matrix[i][j] =
                A_compliments_transponsed.matrix[i][j] / det;
          }
        }

        remove_matrix(&A_compliments);
        remove_matrix(&A_compliments_transponsed);
      } else {
        resulting_code = 2;
      }
    } else {
      resulting_code = 2;
    }
  } else {
    resulting_code = 1;
  }
  return resulting_code;
}
