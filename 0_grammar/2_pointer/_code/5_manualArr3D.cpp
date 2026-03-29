/*
在 C 语言中，`typedef` 关键字用于为现有类型创建新的类型别名。语法规则要求 `typedef` 声明的格式与变量声明的格式相同。具体来说，`typedef` 的语法是：

```c
typedef existing_type new_type_name;
```

### 解释

#### 正确的语法
```c
typedef int type[2];
```
- **解释**：这行代码定义了一个新的类型别名 `type`，它表示一个包含 2 个 `int` 元素的数组。
- **语法规则**：`typedef` 的语法要求 `existing_type` 在前，`new_type_name` 在后。这里 `existing_type` 是 `int[2]`，`new_type_name` 是 `type`。

#### 错误的语法
```c
typedef int[2] type;
```
- **解释**：这行代码的语法是错误的，因为 `typedef` 关键字要求类型声明的格式与变量声明的格式相同。在变量声明中，类型名必须在前，变量名在后。
- **语法错误**：`int[2]` 不是一个有效的类型名，编译器无法解析这种语法。

### 示例
以下是一些使用 `typedef` 的示例，展示了如何为不同类型创建别名：

#### 一维数组
```c
typedef int array_of_two_ints[2];
array_of_two_ints arr = {1, 2};
```
- **解释**：`array_of_two_ints` 是一个包含 2 个 `int` 元素的数组类型。

#### 多维数组
```c
typedef int array_of_two_by_two_ints[2][2];
array_of_two_by_two_ints arr = {{1, 2}, {3, 4}};
```
- **解释**：`array_of_two_by_two_ints` 是一个包含 2x2 个 `int` 元素的二维数组类型。

#### 指针类型
```c
typedef int *int_pointer;
int_pointer p;
```
- **解释**：`int_pointer` 是一个指向 `int` 类型的指针类型。

### 总结
- **语法规则**：`typedef` 的语法要求类型声明的格式与变量声明的格式相同。类型名必须在前，变量名在后。
- **正确的语法**：`typedef int type[2];` 定义了一个新的类型别名 `type`，表示一个包含 2 个 `int` 元素的数组。
- **错误的语法**：`typedef int[2] type;` 是错误的，因为 `int[2]` 不是一个有效的类型名。

通过理解 `typedef` 的语法规则，可以正确地为现有类型创建新的类型别名，提高代码的可读性和可维护性。
*/

#include <cstdlib>
#include <iostream>

const int X_DIM = 16;
const int Y_DIM = 5;
const int Z_DIM = 3;

int main() {
    char *space =
        static_cast<char *>(std::malloc(X_DIM * Y_DIM * Z_DIM * sizeof(char)));
    char ***Arr3D = static_cast<char ***>(std::malloc(Z_DIM * sizeof(char **)));
    for (int z = 0; z < Z_DIM; z++) {
        Arr3D[z] = static_cast<char **>(std::malloc(Y_DIM * sizeof(char *)));
        for (int y = 0; y < Y_DIM; y++) {
            Arr3D[z][y] = space + (z * (X_DIM * Y_DIM) + y * X_DIM);
        }
    }

    ptrdiff_t diff;
    for (int z = 0; z < Z_DIM; z++) {
        std::cout << "Location of array " << z << " is "
                  << static_cast<void *>(*Arr3D[z]) << std::endl;
        for (int y = 0; y < Y_DIM; y++) {
            std::cout << "  Array " << z << " and Row " << y << " starts at "
                      << static_cast<void *>(Arr3D[z][y]);
            diff = Arr3D[z][y] - space;
            std::cout << "    diff = " << diff << "  ";
            std::cout << " z = " << z << "  y = " << y << std::endl;
        }
    }

    // 释放内存
    for (int z = 0; z < Z_DIM; z++) {
        std::free(Arr3D[z]);
    }
    std::free(Arr3D);
    std::free(space);

    return 0;
}