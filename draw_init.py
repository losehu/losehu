from PIL import Image, ImageDraw
import sys

def set_first_16x16_to_zero(file_path):

        # 打开文件以进行写入
        with open(file_path, 'w') as file:
            # 填充前16行16列为'0'
            for i in range(16):
                file.write('0' * 16 + '\n')
            file.write('1' + '\n')
            print("txt init ok")


        


#返回./now.txt的row行col列值
def check_file_content(file_path, row, col):
    # 打开文件
    with open(file_path, 'r') as file:
        lines = file.readlines()
    # 返回指定行列的内容
    return lines[row - 1][col - 1] 

# 创建棋盘函数
def create_chessboard(board_size=15, cell_size=30, padding=20, board_color=(240, 200, 140), line_color=(0, 0, 0)):
    # 创建棋盘图像
    board_image = Image.new("RGB", ((board_size + 1) * cell_size + 2 * padding, (board_size + 1) * cell_size + 2 * padding), board_color)
    draw = ImageDraw.Draw(board_image)

    # 绘制棋盘格线
    for i in range(board_size + 1):
        draw.line((padding, padding + i * cell_size, padding + board_size * cell_size, padding + i * cell_size), fill=line_color, width=2)
        draw.line((padding + i * cell_size, padding, padding + i * cell_size, padding + board_size * cell_size), fill=line_color, width=2)
    set_first_16x16_to_zero('./now.txt')

    return board_image, draw

def set_file_content(file_path, row, col, new_content):
    # 打开文件
    with open(file_path, 'r') as file:
        lines = file.readlines()

    # 行数不足时添加空行
    while row > len(lines):
        lines.append('\n')

    # 列数不足时添加空格
    while col > len(lines[row - 1]):
        lines[row - 1] += ' '

    # 修改指定行列的内容
    lines[row - 1] = lines[row - 1][:col - 1] + new_content + lines[row - 1][col + len(new_content) - 1:]

    # 将修改后的内容写回文件
    with open(file_path, 'w') as file:
        file.writelines(lines)


def add_piece(draw, row, col, color, cell_size=30, padding=20):
    if check_file_content('./now.txt',row+1,col+1)!='0':
        print('not empty')
        return
    x = padding + col * cell_size
    y = padding + row * cell_size
    draw.ellipse((x - cell_size // 2, y - cell_size // 2, x + cell_size // 2, y + cell_size // 2), fill=color)
    set_file_content('./now.txt', row+1, col+1,'1' if color == (0, 0, 0) else '2')
    set_file_content('./now.txt',17, 1,'2' if color == (0, 0, 0) else '1')
    print('black ok' if color == (0, 0, 0) else 'white ok')
    if judge_end()==1:
        set_file_content('./now.txt',17, 1,'3')
        print("black win")
    elif judge_end()==2:
        set_file_content('./now.txt',17, 1,'4')
        print("white win")
    else:
        print("continue")



def read_board(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()[:16]  # 读取前16行
        board = [list(line.strip())[:16] for line in lines]  # 读取前16列并构建棋盘
    return board

def check_five_in_a_row(board, player):
    directions = [(0, 1), (1, 0), (1, 1), (1, -1)]  # 横、竖、斜线方向
    for i in range(len(board)):
        for j in range(len(board[i])):
            if board[i][j] == player:
                for dx, dy in directions:
                    count = 1
                    x, y = i + dx, j + dy
                    while 0 <= x < 16 and 0 <= y < 16 and board[x][y] == player:
                        count += 1
                        x, y = x + dx, y + dy
                    x, y = i - dx, j - dy
                    while 0 <= x < 16 and 0 <= y < 16 and board[x][y] == player:
                        count += 1
                        x, y = x - dx, y - dy
                    if count >= 5:
                        return True
    return False  

def judge_end():
    board = read_board('./now.txt')
    if check_five_in_a_row(board, '1'):
        return 1
    elif check_five_in_a_row(board, '2'):
        return 2
    else:
        return 0







# 写一个main
if __name__ == '__main__':

    row=int(sys.argv[1])
    col=int(sys.argv[2])



    if row>16 or col>16:
        print('out of range')
        sys.exit()  # 退出程序

    board_image=Image.open("chessboard.png")
    draw=ImageDraw.Draw(board_image)
    if check_file_content('./now.txt', 17, 1)=='1':

        add_piece(draw, row-1, col-1, (0, 0, 0))# 在指定位置添加黑/白棋子
    elif check_file_content('./now.txt', 17, 1)=='2':
        add_piece(draw, row-1, col-1, (255,255,255))# 在指定位置添加黑/白棋子
    else : #win
        board_image, draw = create_chessboard()# 创建空白棋盘
        add_piece(draw, row-1, col-1, (0, 0, 0))# 在指定位置添加黑/白棋子
    board_image.save("chessboard.png")# 保存棋盘图像
    
