from enum import Enum

aa_dict = {
        'A':0,
        'R':1,
        'N':2,
        'D':3,
        'C':4,
        'Q':5,
        'E':6,
        'G':7,
        'H':8,
        'I':9,
        'L':10,
        'K':11,
        'M':12,
        'F':13,
        'P':14,
        'S':15,
        'T':16,
        'W':17,
        'Y':18,
        'V':19,
        'B':20,
        'Z':21,
        'X':22,
        '*':23 
        }

class Arrow(Enum):
    UP = 1
    LEFT = 2
    NW = 3

def get_reward(fchar : str, schar : str, gap_start : int, gap_continue, blosum):
    return int(blosum[aa_dict[fchar]][aa_dict[schar]])

def calculate_score(first : str, second : str, gap_open : int, gap_continue):
    score = 0
    if(len(first) != len(second)):
        print("LENGTHS NOT EQUAL")
        exit()
    for a in range(len(first)):
        score += get_reward(first[a], second[a], gap_open, gap_continue)
    return score

def get_max(a : int, b : int, c : int):
    result_val = ([], max(a, b, c))
    if(a == result_val[1]):
        result_val[0].append(Arrow.NW)
    if(b == result_val[1]):
        result_val[0].append(Arrow.LEFT)
    if(c == result_val[1]):
        result_val[0].append(Arrow.UP)
    return result_val

def get_alignment(matrix, a : str, b : str, x : int, y : int):
    fw1, fw2 = "", ""
    while(x != 0 or y != 0):
        #regular flow
        if(matrix[y][x][0][0] == Arrow.NW):
            fw1 = a[x - 1] + fw1
            fw2 = b[y - 1] + fw2
            y -= 1
            x -= 1
        elif(matrix[y][x][0][0] == Arrow.LEFT):
            fw1 = a[x - 1] + fw1
            fw2 = "-" + fw2
            x -= 1
        elif(matrix[y][x][0][0] == Arrow.UP):
            fw1 = "-" + fw1
            fw2 = b[y - 1] + fw2
            y -= 1
    return fw1, fw2

def make_alignment(first : str, second : str, gap_start : int, gap_continue : int):
    # Read the blosum62 matrix for gap and match scoring
    blosum = open("weights.csv")
    blosum = [x.split(",") for x in blosum.readlines()]
    for elem in blosum:
        elem.remove("\n")
    # Trivial cases are handled by the current code (one or two empty sequences)
    # so I've decided not to include them as separate IFs
    v_matrix = [[None for x in range(len(first) + 1)] for y in range(len(second) + 1)]
    # Fill the matrix following with dynamic calculations
    for y in range(len(second) + 1):
        for x in range(len(first) + 1):
            if(x == 0 and y == 0):
                v_matrix[y][x] = ([], 0)
                continue
            elif(x == 0):
                v_matrix[y][x] = ([Arrow.UP], gap_start + y * gap_continue)
                continue
            elif(y == 0):
                v_matrix[y][x] = ([Arrow.LEFT], gap_start + x * gap_continue)
                continue
            a = v_matrix[y-1][x-1][1] + get_reward(first[x - 1], second[y - 1], gap_start, gap_continue, blosum)
            b = 0
            for k in range(0, x):
                curr = v_matrix[y][k][1] - gap_start + gap_continue * (x - k)
                if curr > b:
                    b = curr
            c = 0
            for k in range(0, y):
                curr = v_matrix[k][x][1] - gap_start + gap_continue * (y - k)
                if curr > c:
                    c = curr
            v_matrix[y][x] = get_max(a, b, c)

    # Alignment reconstruction to find only ONE
    # optimal alignment string, to find all 
    # it would be sufficient to tweak the get_alignment
    # function to a recursive one returning an array of possible 
    # alignments with memoization employed for better performance
    # purposes
    i, j= len(first), len(second)
    res = get_alignment(v_matrix, first, second, i, j)
    return res[0], res[1], v_matrix[len(second)][len(first)][1]

    
