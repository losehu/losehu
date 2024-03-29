#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "string.h"

using namespace std;
const std::string filename = "../Readme.md";


#define SIZE 4

long long int board[SIZE][SIZE];
int start_line = 0;

// 初始化游戏面板
void initialize() {
    long long int i, j;
    for (i = 0; i < SIZE; ++i) {
        for (j = 0; j < SIZE; ++j) {
            board[i][j] = 0;
        }
    }
    // 随机生成两个初始数字
    srand(time(NULL));
    long long int r1 = rand() % SIZE;
    long long int c1 = rand() % SIZE;
    long long int r2 = rand() % SIZE;
    long long int c2 = rand() % SIZE;
    while (r1 == r2 && c1 == c2) {
        r2 = rand() % SIZE;
        c2 = rand() % SIZE;
    }
    board[r1][c1] = 2;
    board[r2][c2] = 2;
}

// 在随机空白位置生成一个新数字2或4
void generateNewNumber() {
    long long int r, c;
    long long int newNumber = (rand() % 2 + 1) * 2; // 生成2或4
    do {
        r = rand() % SIZE;
        c = rand() % SIZE;
    } while (board[r][c] != 0);
    board[r][c] = newNumber;
}

long long int getPowerOfTwo(long long int n) {
    long long int count = 0;

    // 如果一个数是2的幂次方，则其二进制表示中只有一位是1，其余位都是0
    // 统计该数二进制表示中1的个数即为其幂次方数
    while (n > 0) {

        count++;
        n >>= 1; // 右移一位，相当于除以2
    }

    return count - 1; // 减1是因为最后一次右移得到的是0，对应着2^0，而不是2^1
}

bool overwriteLine(const std::string &filename, int lineNumber, const char *content);

// 打印游戏面板
void printBoard() {
    long long int i, j;
    for (i = 0; i < SIZE; ++i) {
        for (j = 0; j < SIZE; ++j) {
            printf("%4d", board[i][j]);
        }
        printf("\n");
    }


    // 覆盖第n行的内容为char数组
    int now_line = start_line + 1;
    for (int k = 0; k < 4; ++k) {
        char newContent[160] = "<div class=\"grid-item\"><img src=\"./img/00001.svg\" alt=\"Image 1\"></div>";

        for (int l = 0; l < 4; ++l) {
            if (board[k][l] == 0) {
                newContent[39] = 'b';
                newContent[40] = 'l';
                newContent[41] = 'a';
                newContent[42] = 'n';
                newContent[43] = 'k';

            } else {

                newContent[39] = '0';
                newContent[40] = '0';
                newContent[41] = '0';
                newContent[42] = '0' + getPowerOfTwo(board[k][l]) / 10;
                newContent[43] = '0' + getPowerOfTwo(board[k][l]) % 10;
            }
            overwriteLine(filename, now_line, newContent);
            now_line++;

        }


    }

}

bool moveUp() {
    long long int i, j, k;
    // 逐列处理
    bool move = false;

    for (j = 0; j < SIZE; ++j) {
        // 先把所有数字方块往上移动
        for (i = 1; i < SIZE; ++i) {
            if (board[i][j] != 0) {
                // 找到上面第一个非空格
                for (k = i - 1; k >= 0; --k) {
                    if (board[k][j] == 0) {
                        // 移动数字方块
                        board[k][j] = board[k + 1][j];
                        board[k + 1][j] = 0;
                        move = true;

                    } else if (board[k][j] == board[k + 1][j]) {
                        // 合并相同数字方块
                        board[k][j] *= 2;
                        board[k + 1][j] = 0;
                        move = true;

                        break;
                    } else {
                        // 不能合并，退出循环
                        break;
                    }
                }
            }
        }
    }
    generateNewNumber();

    if (!move)return false;
}

// 向下移动
bool moveDown() {
    long long int i, j, k;
    bool move = false;
    // 逐列处理

    for (j = 0; j < SIZE; ++j) {
        // 先把所有数字方块往下移动
        for (i = SIZE - 2; i >= 0; --i) {
            if (board[i][j] != 0) {
                // 找到下面第一个非空格
                for (k = i + 1; k < SIZE; ++k) {
                    if (board[k][j] == 0) {
                        // 移动数字方块
                        board[k][j] = board[k - 1][j];
                        board[k - 1][j] = 0;
                        move = true;
                    } else if (board[k][j] == board[k - 1][j]) {
                        // 合并相同数字方块
                        board[k][j] *= 2;
                        board[k - 1][j] = 0;
                        move = true;

                        break;
                    } else {
                        // 不能合并，退出循环
                        break;
                    }
                }
            }
        }
    }
    generateNewNumber();

    if (!move) return false;

}

// 向左移动
bool moveLeft() {
    long long int i, j, k;
    bool move = false;

    // 逐行处理
    for (i = 0; i < SIZE; ++i) {
        // 先把所有数字方块往左移动
        for (j = 1; j < SIZE; ++j) {
            if (board[i][j] != 0) {
                // 找到左边第一个非空格
                for (k = j - 1; k >= 0; --k) {
                    if (board[i][k] == 0) {
                        // 移动数字方块
                        board[i][k] = board[i][k + 1];
                        board[i][k + 1] = 0;
                        move = true;
                    } else if (board[i][k] == board[i][k + 1]) {
                        // 合并相同数字方块
                        board[i][k] *= 2;
                        board[i][k + 1] = 0;
                        move = true;

                        break;
                    } else {
                        // 不能合并，退出循环
                        break;
                    }
                }
            }
        }
    }
    generateNewNumber();

    if (!move)return false;

}

// 向右移动
bool moveRight() {
    long long int i, j, k;
    bool move = false;

    // 逐行处理
    for (i = 0; i < SIZE; ++i) {
        // 先把所有数字方块往右移动
        for (j = SIZE - 2; j >= 0; --j) {
            if (board[i][j] != 0) {
                // 找到右边第一个非空格
                for (k = j + 1; k < SIZE; ++k) {
                    if (board[i][k] == 0) {
                        // 移动数字方块
                        board[i][k] = board[i][k - 1];
                        board[i][k - 1] = 0;
                        move = true;
                    } else if (board[i][k] == board[i][k - 1]) {
                        // 合并相同数字方块
                        board[i][k] *= 2;
                        board[i][k - 1] = 0;
                        move = true;

                        break;
                    } else {
                        // 不能合并，退出循环

                        break;
                    }
                }
            }
        }
    }
    generateNewNumber();

    if (!move)return false;

}

// 检查游戏是否结束
bool isGameOver() {
    long long int i, j;

    // 检查是否有空格子
    for (i = 0; i < SIZE; ++i) {
        for (j = 0; j < SIZE; ++j) {
            if (board[i][j] == 0) {
                return false; // 还有空格子，游戏未结束
            }
        }
    }

    return true; // 所有格子被填满且不能再合并，游戏结束
}

//计算总分
long long int totalScore() {
    long long int i, j;
    long long int score = 0;
    for (i = 0; i < SIZE; ++i) {
        for (j = 0; j < SIZE; ++j) {
            score += board[i][j];
        }
    }
    return score;
}

// 查找txt中特定字符串所在的行号
std::vector<int> findLinesWithText(const std::string &filename, const std::string &searchText) {
    std::ifstream file(filename);
    std::vector<int> lines;
    std::string line;
    int lineNumber = 1;

    while (std::getline(file, line)) {
        if (line.find(searchText) != std::string::npos) {
            lines.push_back(lineNumber);
        }
        lineNumber++;
    }

    return lines;
}


bool overwriteLine(const std::string &filename, int lineNumber, const char *content) {
    std::fstream file(filename);
    std::string line;
    std::vector<std::string> lines;

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    // Read all lines into memory
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    file.close();

    // Check if lineNumber is valid
    if (lineNumber < 1 || lineNumber > lines.size()) {
        std::cerr << "Line " << lineNumber << " does not exist in the file." << std::endl;
        return false;
    }

    // Overwrite the content
    lines[lineNumber - 1] = content;

    // Rewrite the file with updated content
    file.open(filename, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return false;
    }

    for (const auto &l: lines) {
        file << l << std::endl;
    }

    return true;
}


int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("error num %d \n", argv[0]);
        return 1;
    }
    long long int input = 0; // 将字符串转换为整数

    if (strcmp(argv[1], "NEW") == 0) {
        input = 5;
    } else if (strcmp(argv[1], "UP") == 0) {
        input = 8;
    } else if (strcmp(argv[1], "DOWN") == 0) {
        input = 2;
    } else if (strcmp(argv[1], "LEFT") == 0) {
        input = 4;
    } else if (strcmp(argv[1], "RIGHT") == 0) {
        input = 6;
    } else {
        printf("error num ");
        return 1;
    }


    const std::string searchText = "<div class=\"grid-container\">";
    const char *newContent = "This line is overwritten.";

    // 查找特定字符串所在的行号
    std::vector<int> lines = findLinesWithText(filename, searchText);
    std::cout << "Lines containing '" << searchText << "': ";
    for (int line: lines) {
        std::cout << line << " ";
    }
    std::cout << std::endl;
    start_line = lines[0];
    bool gameover = false;
    initialize();
    printf("score %d\n", totalScore());
    printBoard();
    bool state = false;
    switch (input) {
        case 8:
            state = moveUp();
            break;
        case 2:
            state = moveDown();
            break;
        case 4:
            state = moveLeft();
            break;
        case 6:
            state = moveRight();
            break;
        case 5:
            initialize();
            break;

    }
    if (!state && (input == 8 || input == 2 || input == 4 || input == 6) && isGameOver()) {
        printf("Game Over!\n");
        gameover = true;
        initialize();
    }
    return 0;
}
