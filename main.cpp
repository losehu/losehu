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
const std::string filename = "./Readme.md";


#define SIZE 4

long long int board[SIZE][SIZE];
int start_line = 0;

// ��ʼ����Ϸ���
void initialize() {
    long long int i, j;
    for (i = 0; i < SIZE; ++i) {
        for (j = 0; j < SIZE; ++j) {
            board[i][j] = 0;
        }
    }
    // �������������ʼ����
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

// ������հ�λ������һ��������2��4
void generateNewNumber() {
    long long int r, c;
    long long int newNumber = (rand() % 2 + 1) * 2; // ����2��4
    do {
        r = rand() % SIZE;
        c = rand() % SIZE;
    } while (board[r][c] != 0);
    board[r][c] = newNumber;
}

long long int getPowerOfTwo(long long int n) {
    long long int count = 0;

    // ���һ������2���ݴη�����������Ʊ�ʾ��ֻ��һλ��1������λ����0
    // ͳ�Ƹ��������Ʊ�ʾ��1�ĸ�����Ϊ���ݴη���
    while (n > 0) {

        count++;
        n >>= 1; // ����һλ���൱�ڳ���2
    }

    return count - 1; // ��1����Ϊ���һ�����Ƶõ�����0����Ӧ��2^0��������2^1
}

bool overwriteLine(const std::string &filename, int lineNumber, const char *content);

// ��ӡ��Ϸ���
void flashboard() {
    long long int i, j;
    for (i = 0; i < SIZE; ++i) {
        for (j = 0; j < SIZE; ++j) {
            printf("%4d", board[i][j]);
        }
        printf("\n");
    }


    // ���ǵ�n�е�����Ϊchar����
    int now_line = start_line + 2;
    for (int k = 0; k < 4; ++k) {
        char newContent[170] = "| <img src=\"./img/blank.svg\" width=100px> | <img src=\"./img/blank.svg\" width=100px> | <img src=\"./img/blank.svg\" width=100px> | <img src=\"./img/blank.svg\" width=100px> |";

        for (int l = 0; l < 4; ++l) {
            if (board[k][l] == 0) {
                newContent[18 + l * 42] = 'b';
                newContent[19 + l * 42] = 'l';
                newContent[20 + l * 42] = 'a';
                newContent[21 + l * 42] = 'n';
                newContent[22 + l * 42] = 'k';
            } else {
                newContent[18 + l * 42] = '0';
                newContent[19 + l * 42] = '0';
                newContent[20 + l * 42] = '0';
                newContent[21 + l * 42] = '0' + getPowerOfTwo(board[k][l]) / 10;
                newContent[22 + l * 42] = '0' + getPowerOfTwo(board[k][l]) % 10;
            }


        }
        overwriteLine(filename, now_line, newContent);
        if (k == 0) now_line += 2;
        else now_line += 1;

    }

}

bool moveUp() {
    long long int i, j, k;
    // ���д���
    bool move = false;

    for (j = 0; j < SIZE; ++j) {
        // �Ȱ��������ַ��������ƶ�
        for (i = 1; i < SIZE; ++i) {
            if (board[i][j] != 0) {
                // �ҵ������һ���ǿո�
                for (k = i - 1; k >= 0; --k) {
                    if (board[k][j] == 0) {
                        // �ƶ����ַ���
                        board[k][j] = board[k + 1][j];
                        board[k + 1][j] = 0;
                        move = true;

                    } else if (board[k][j] == board[k + 1][j]) {
                        // �ϲ���ͬ���ַ���
                        board[k][j] *= 2;
                        board[k + 1][j] = 0;
                        move = true;

                        break;
                    } else {
                        // ���ܺϲ����˳�ѭ��
                        break;
                    }
                }
            }
        }
    }
    generateNewNumber();

    if (!move)return false;
}

// �����ƶ�
bool moveDown() {
    long long int i, j, k;
    bool move = false;
    // ���д���

    for (j = 0; j < SIZE; ++j) {
        // �Ȱ��������ַ��������ƶ�
        for (i = SIZE - 2; i >= 0; --i) {
            if (board[i][j] != 0) {
                // �ҵ������һ���ǿո�
                for (k = i + 1; k < SIZE; ++k) {
                    if (board[k][j] == 0) {
                        // �ƶ����ַ���
                        board[k][j] = board[k - 1][j];
                        board[k - 1][j] = 0;
                        move = true;
                    } else if (board[k][j] == board[k - 1][j]) {
                        // �ϲ���ͬ���ַ���
                        board[k][j] *= 2;
                        board[k - 1][j] = 0;
                        move = true;

                        break;
                    } else {
                        // ���ܺϲ����˳�ѭ��
                        break;
                    }
                }
            }
        }
    }
    generateNewNumber();

    if (!move) return false;

}

// �����ƶ�
bool moveLeft() {
    long long int i, j, k;
    bool move = false;

    // ���д���
    for (i = 0; i < SIZE; ++i) {
        // �Ȱ��������ַ��������ƶ�
        for (j = 1; j < SIZE; ++j) {
            if (board[i][j] != 0) {
                // �ҵ���ߵ�һ���ǿո�
                for (k = j - 1; k >= 0; --k) {
                    if (board[i][k] == 0) {
                        // �ƶ����ַ���
                        board[i][k] = board[i][k + 1];
                        board[i][k + 1] = 0;
                        move = true;
                    } else if (board[i][k] == board[i][k + 1]) {
                        // �ϲ���ͬ���ַ���
                        board[i][k] *= 2;
                        board[i][k + 1] = 0;
                        move = true;

                        break;
                    } else {
                        // ���ܺϲ����˳�ѭ��
                        break;
                    }
                }
            }
        }
    }
    generateNewNumber();

    if (!move)return false;

}

// �����ƶ�
bool moveRight() {
    long long int i, j, k;
    bool move = false;

    // ���д���
    for (i = 0; i < SIZE; ++i) {
        // �Ȱ��������ַ��������ƶ�
        for (j = SIZE - 2; j >= 0; --j) {
            if (board[i][j] != 0) {
                // �ҵ��ұߵ�һ���ǿո�
                for (k = j + 1; k < SIZE; ++k) {
                    if (board[i][k] == 0) {
                        // �ƶ����ַ���
                        board[i][k] = board[i][k - 1];
                        board[i][k - 1] = 0;
                        move = true;
                    } else if (board[i][k] == board[i][k - 1]) {
                        // �ϲ���ͬ���ַ���
                        board[i][k] *= 2;
                        board[i][k - 1] = 0;
                        move = true;

                        break;
                    } else {
                        // ���ܺϲ����˳�ѭ��

                        break;
                    }
                }
            }
        }
    }
    generateNewNumber();

    if (!move)return false;

}

// �����Ϸ�Ƿ����
bool isGameOver() {
    long long int i, j;

    // ����Ƿ��пո���
    for (i = 0; i < SIZE; ++i) {
        for (j = 0; j < SIZE; ++j) {
            if (board[i][j] == 0) {
                return false; // ���пո��ӣ���Ϸδ����
            }
        }
    }

    return true; // ���и��ӱ������Ҳ����ٺϲ�����Ϸ����
}

//�����ܷ�
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

// ����txt���ض��ַ������ڵ��к�
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

void READboard() {
    std::ifstream file(filename);
    std::vector<int> lines;
    std::string line;

    for (int i = 0; i < start_line + 1; i++) {
        std::getline(file, line);
    }
    for (int i = 0; i < 4; ++i) {
        std::getline(file, line);
        if (i == 1)std::getline(file, line);
        cout<<line<<endl;
        for (int j = 0; j < 4; j++) {
            if (line[18 + j * 42] == 'b') {
                board[i][j] = 0;
            } else {
                board[i][j] = 2<<(10* (line[21 + j * 42] - '0') +  (line[22 + j * 42] - '0')-1);
            }
        }

    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j <4 ; ++j) {
            printf("%d ",board[i][j]);
        }
        printf("\n");
    }
}


int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("error num %d \n", argv[0]);
        return 1;
    }
    long long int input = 0; // ���ַ���ת��Ϊ����

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


    const std::string searchText = "<!-- BEGIN CHESS BOARD -->";

    // �����ض��ַ������ڵ��к�
    std::vector<int> lines = findLinesWithText(filename, searchText);
    std::cout << "Lines containing '" << searchText << "': ";
    for (int line: lines) {
        std::cout << line << " ";
    }
    std::cout << std::endl;
    start_line = lines[0];

    printf("score %d\n", totalScore());
    READboard();

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
        initialize();
    }
    flashboard();
    char str[50];
    sprintf(str,"score: **%lld**\n", totalScore());
    overwriteLine(filename, start_line-1, str);
    return 0;
}
