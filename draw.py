from PIL import Image, ImageDraw,ImageFont
import sys
import re
import requests
import random
import string
import os
random_string=None
def set_first_15x15_to_zero(file_path):

        # 打开文件以进行写入
        with open(file_path, 'w') as file:
            # 填充前15行15列为'0'
            for i in range(15):
                file.write('0' * 15 + '\n')
            file.write('1' + '\n')
            comment_on_issue( sys.argv[1], "A new game start!", sys.argv[3])

            


        


#返回./now.txt的row行col列值
def check_file_content(file_path, row, col):
    # 打开文件
    with open(file_path, 'r') as file:
        lines = file.readlines()
    # 返回指定行列的内容
    return lines[row - 1][col - 1] 

# 创建棋盘函数
def create_chessboard(board_size=14, cell_size=30, padding=20, board_color=(240, 200, 140), line_color=(0, 0, 0)):
    # 创建棋盘图像
    board_image = Image.new("RGB", ((board_size + 1) * cell_size + 2 * padding, (board_size + 1) * cell_size + 2 * padding), board_color)
    draw = ImageDraw.Draw(board_image)

    # 绘制棋盘格线
    for i in range(board_size + 1):
        draw.line((padding+15, 15+padding + i * cell_size, 15+padding + board_size * cell_size, 15+padding + i * cell_size), fill=line_color, width=2)
        draw.line((padding +15+ i * cell_size,15+ padding,15+ padding + i * cell_size, 15+padding + board_size * cell_size), fill=line_color, width=2)
    
        # 写入行号和列号
    font = ImageFont.load_default()  # 使用默认字体
    for i in range(board_size+1):
        row_number = "{:02d}".format(i + 1) if i + 1 < 10 else str(i + 1)
        col_number = "{:02d}".format(i + 1) if i + 1 < 10 else str(i + 1)

        draw.text((padding - 15, padding+10 + i * cell_size),row_number, fill=line_color, font=font)  # 左侧列号
        draw.text((padding +15+3+ (board_size + 1) * cell_size -15, padding +10+ i * cell_size), row_number, fill=line_color, font=font)  # 右侧列号
        draw.text((padding + i * cell_size+12, padding - 15),col_number, fill=line_color, font=font)  # 上方行号
        draw.text((padding + i * cell_size+12,10+15+ padding + (board_size + 1) * cell_size -20), col_number, fill=line_color, font=font)  # 下方行号
    
    set_first_15x15_to_zero('./now.txt')
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
        comment_on_issue( sys.argv[1], "This move has already been made.", sys.argv[3])
        return
    x = padding + col * cell_size
    y = padding + row * cell_size

    # draw.ellipse((x - cell_size // 2+15, 15+y - cell_size // 2, 15+x + cell_size // 2,15+ y + cell_size // 2), fill=color)
    print(x - cell_size // 2+15, 15+y - cell_size // 2, 15+x + cell_size // 2,15+ y + cell_size // 2)
    set_file_content('./now.txt', row+1, col+1,'1' if color == (0, 0, 0) else '2')
    set_file_content('./now.txt',16, 1,'2' if color == (0, 0, 0) else '1')
    if judge_end()==1:
        set_file_content('./now.txt',16, 1,'3')
        comment_on_issue( sys.argv[1], "black win", sys.argv[3])

    elif judge_end()==2:
        set_file_content('./now.txt',16, 1,'4')
        comment_on_issue( sys.argv[1], "white win", sys.argv[3])
    else:
        pass
    board=read_board('./now.txt')
    for i in range(len(board)):
        for j in range(len(board[i])):
            x = padding + j * cell_size
            y = padding + i * cell_size

            if board[i][j] == '1':
                draw.ellipse((x - (cell_size-5) // 2+15, 15+y -  (cell_size-5) // 2, 15+x +  (cell_size-5) // 2,15+ y +  (cell_size-5) // 2), fill=(0,0,0))
            elif board[i][j] == '2':
                draw.ellipse((x -  (cell_size-5) // 2+15, 15+y -  (cell_size-5) // 2, 15+x +  (cell_size-5) // 2,15+ y +  (cell_size-5) // 2), fill=(255,255,255))






def read_board(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()[:15]  # 读取前15行
        board = [list(line.strip())[:15] for line in lines]  # 读取前15列并构建棋盘
    return board

def check_five_in_a_row(board, player):
    directions = [(0, 1), (1, 0), (1, 1), (1, -1)]  # 横、竖、斜线方向
    for i in range(len(board)):
        for j in range(len(board[i])):
            if board[i][j] == player:
                for dx, dy in directions:
                    count = 1
                    x, y = i + dx, j + dy
                    while 0 <= x < 15 and 0 <= y < 15 and board[x][y] == player:
                        count += 1
                        x, y = x + dx, y + dy
                    x, y = i - dx, j - dy
                    while 0 <= x < 15 and 0 <= y < 15 and board[x][y] == player:
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



def extract_numbers(input_str):
    # 定义正则表达式模式
    pattern = r'^(\d{1,2}|1[0-6]) +(\d{1,2}|1[0-6])$'

    # 使用正则表达式进行匹配
    match = re.match(pattern, input_str)

    # 如果匹配成功，返回两个数字，否则返回 None
    if match:
        return int(match.group(1)), int(match.group(2))
    else:
        return None,None


def comment_on_issue( issue_number, comment_body, access_token):
    url = f"https://api.github.com/repos/losehu/losehu/issues/{issue_number}/comments"
    headers = {
        "Authorization": f"token {access_token}",
        "Accept": "application/vnd.github.v3+json"
    }
    payload = {
        "body": comment_body
    }
    print("Delete URL:", url)  # 打印URL

    response = requests.post(url, json=payload, headers=headers)
    if response.status_code == 201:
        print("Comment posted successfully.")
    else:
        print(f"Failed to post comment: {response.status_code} - {response.text}")


def update_readme():
    global random_string
    c= check_file_content('./now.txt', 16, 1)
    with open('./Readme.md', 'r+') as file:
        lines = file.readlines()
        for i in range(len(lines)):
            if "<!-- flag -->" in lines[i]:
                if c=='1':
                    lines[i-2] = "**Black's** turn now.\n"
                elif c=='2':
                    lines[i-2] = "**White's** turn now.\n"
                elif  c=='3':
                    lines[i-2] = "**Black's** WIN!.\n"
                elif  c=='4':
                    lines[i-2] = "**White's** WIN!.\n"
                lines[i+1]="![chessboard](./"+random_string+".png)"
                break
        file.seek(0)
        file.writelines(lines)
        file.truncate()


def del_pic():
    for file in os.listdir('.'):
        if file.endswith('.png') and file != 'chessboard_empty.png':
            print(file)
            os.remove(file)

# 写一个main
if __name__ == '__main__':
    # #随机生成长度为8的字符串
    # random_string = ''.join(random.choices(string.ascii_letters + string.digits, k=8))
    # del_pic()
    # board_image, draw = create_chessboard()# 创建空白棋盘
    # board_image.save(random_string+".png")# 保存棋盘图像
    # update_readme()
    # sys.exit(0)

    row,col=extract_numbers(sys.argv[2])
   
    if row==None:
        comment_on_issue( sys.argv[1], "error input", sys.argv[3])
        sys.exit()  # 退出程序


    if row>15 or col>15:
        comment_on_issue( sys.argv[1], "out of range(1~15)", sys.argv[3])
        sys.exit()  # 退出程序


    #随机生成长度为8的字符串
    random_string = ''.join(random.choices(string.ascii_letters + string.digits, k=8))
    del_pic()

    board_image=Image.open("chessboard_empty.png")
    draw=ImageDraw.Draw(board_image)
    if check_file_content('./now.txt', 16, 1)=='1':
        add_piece(draw, row-1, col-1, (0, 0, 0))# 在指定位置添加黑/白棋子
    elif check_file_content('./now.txt', 16, 1)=='2':
        add_piece(draw, row-1, col-1, (255,255,255))# 在指定位置添加黑/白棋子
    else : #win
        board_image, draw = create_chessboard()# 创建空白棋盘
        add_piece(draw, row-1, col-1, (0, 0, 0))# 在指定位置添加黑/白棋子
    
    board_image.save(random_string+".png")# 保存棋盘图像



    comment_on_issue( sys.argv[1], "Successful, thank you for your assistance.", sys.argv[3])
    update_readme()
    



