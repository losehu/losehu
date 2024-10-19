#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
const std::string filename = "./Readme.md";

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
void flashboard() {
    long long int i, j;
    for (i = 0; i < SIZE; ++i) {
        for (j = 0; j < SIZE; ++j) {
            printf("%4lld", board[i][j]);
        }
        printf("\n");
    }

    // 覆盖第n行的内容为char数组
    int now_line = start_line + 2;
    for (int k = 0; k < 4; ++k) {
        char newContent[170] = "| <img src=\"./img/blank.png\" width=100px> | <img src=\"./img/blank.png\" width=100px> | <img src=\"./img/blank.png\" width=100px> | <img src=\"./img/blank.png\" width=100px> |";

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
        if (k == 0)
            now_line += 2;
        else
            now_line += 1;
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

    if (!move)
        return false;
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

    if (!move)
        return false;
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

    if (!move)
        return false;
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

    if (!move)
        return false;
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

// 计算总分
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

#include <iostream>
#include <fstream>
#include <vector>

bool overwriteLine(const std::string &filename, int lineNumber, const char *content) {
    std::fstream file(filename);
    std::string line;
    std::vector<std::string> lines;

    // 打开文件
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    // 读取所有行内容
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    file.close();

    // 如果指定的行号超出文件当前行数，填充空行
    if (lineNumber > lines.size()) {
        lines.resize(lineNumber, "");  // 使用空字符串填充
    }

    // 覆盖指定行的内容
    lines[lineNumber - 1] = content;

    // 打开文件，写回修改后的内容
    file.open(filename, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return false;
    }

    // 将所有行重新写回文件
    for (const auto &l: lines) {
        file << l << std::endl;
    }

    file.close();
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
        if (i == 1)
            std::getline(file, line);
        cout << line << endl;
        for (int j = 0; j < 4; j++) {
            if (line[18 + j * 42] == 'b') {
                board[i][j] = 0;
            } else {
                board[i][j] = 2 << (10 * (line[21 + j * 42] - '0') + (line[22 + j * 42] - '0') - 1);
            }
        }
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            printf("%lld ", board[i][j]);
        }
        printf("\n");
    }
}

int find_name(string name) {
    std::vector<int> lines1 = findLinesWithText(filename, name);
    if (lines1.size() == 0)
        return 0;
    else
        return lines1[0];
}

std::string readTxtLine(const std::string &filename, int row) {
    std::ifstream file(filename);
    std::string line;
    int currentRow = 0;

    while (std::getline(file, line)) {
        currentRow++;

        if (currentRow == row) {
            return line;
        }
    }

    return ""; // 如果行超出范围，返回空字符串
}

struct PlayerInfo {
    std::string name;
    int times;
    int scores;
};

PlayerInfo parseString(const std::string &str) {
    PlayerInfo player;
    std::stringstream ss(str);
    std::string token;

    // 解析格式: | 若干空格 name 若干空格 | 若干空格 times 若干空格 | 若干空格 score 若干空格 |

    // 跳过第一个 '|'
    ss >> std::ws;
    std::getline(ss, token, '|');

    // 跳过若干空格并读取姓名
    ss >> std::ws;
    std::getline(ss, player.name, '|');

    // 跳过若干空格并读取次数
    ss >> std::ws;
    std::getline(ss, token, '|');
    player.times = std::stoi(token);

    // 跳过若干空格并读取分数
    ss >> std::ws;
    std::getline(ss, token, '|');
    player.scores = std::stoi(token);

    return player;
}

int parseNumber(const std::string &str) {
    int number = 0;
    std::stringstream ss(str);
    std::string token;

    // 查找数字部分
    while (std::getline(ss, token, ':')) {
        if (token.find("num") != std::string::npos) {
            // 找到了 "num"，接着读取数字
            std::getline(ss, token, ':');
            number = std::stoi(token);
            break;
        }
    }

    return number;
}

#include <iostream>
#include <regex>
#include <string>

int parseMiddleNumber(const std::string &input) {
    // 定义正则表达式模式，匹配数字
    std::regex pattern("\\d+");

    // 在输入字符串中搜索匹配的内容
    std::smatch matches;
    if (std::regex_search(input, matches, pattern)) {
        // 返回匹配到的第一个数字
        return std::stoi(matches[0]);
    } else {
        // 如果未找到匹配的数字，返回默认值 -1
        return -1;
    }
}

void UPDATE_RANK(char *name) {
    cout << "ok1" << endl;

    int now_line = find_name(name);
    char newContent[160];

    if (now_line) {
        cout << "ok2" << endl;

        string str = readTxtLine(filename, now_line);
        PlayerInfo player = parseString(str);
        player.scores = totalScore() > player.scores ? totalScore() : player.scores;
        sprintf(newContent, "| %s | %d | %d |", name, player.times + 1, player.scores);
        overwriteLine(filename, now_line, newContent);
    } else {
        cout << "ok3" << endl;

        now_line = find_name("<!-- rank -->");
        string str = readTxtLine(filename, now_line - 1);
        int have = parseNumber(str);
        sprintf(newContent, "| %s | %d | %lld |", name, 1, totalScore());
        overwriteLine(filename, now_line + 3 + have, newContent);
        sprintf(newContent, "<!-- num:%d -->", have + 1);
        overwriteLine(filename, now_line - 1, newContent);
    }
    int score_max_line = find_name("score:");
    string str = readTxtLine(filename, score_max_line - 2);

    long long int now_max = parseMiddleNumber(str);
    char str1[50];
    if (now_max < totalScore()) {
        sprintf(str1, "MAX SCORE: **%lld**", totalScore());
        overwriteLine(filename, score_max_line - 2, str1);
    }
}

int main(int argc, char *argv[]) {

    // return 0;

    srand(time(NULL));


    if (argc != 3) {
        printf("error num %s \n", argv[0]);
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

    const std::string searchText = "<!-- BEGIN CHESS BOARD -->";

    // 查找特定字符串所在的行号
    std::vector<int> lines = findLinesWithText(filename, searchText);
    std::cout << "Lines containing '" << searchText << "': ";
    for (int line: lines) {
        std::cout << line << " ";
    }
    std::cout << std::endl;
    start_line = lines[0];

    printf("score %lld\n", totalScore());
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
    sprintf(str, "score: **%lld**", totalScore());
    overwriteLine(filename, start_line - 1, str);
    char str_find[50];
    sprintf(str_find, " %s ", argv[2]);
    UPDATE_RANK(str_find);

    return 0;
}
