#include <stdlib.h>

namespace ManualCreateArr2D {

#define COLS 5
    const int ROWS = 10;
    typedef int RowArray[COLS];
    void example() {
        RowArray *rptr =
            static_cast<RowArray *>(malloc(ROWS * COLS * sizeof(int)));

        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                rptr[row][col] = 8;
            }
        }
    }

}  // namespace ManualCreateArr2D

namespace identifier {
    const int nrows = 10;
    const int ncols = 5;

    void example1() {
        int *aptr = static_cast<int *>(malloc(nrows * ncols * sizeof(int)));
        int **rptr = static_cast<int **>(malloc(nrows * sizeof(int *)));
        for (int k = 0; k < nrows; k++) {
            rptr[k] = aptr + (k * ncols);
        }
    }
}  // namespace identifier

int main(void) {
    ManualCreateArr2D::example();
    identifier::example1();
    return 0;
}