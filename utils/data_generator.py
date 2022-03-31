import random
from typing import List

alphabet = "abcdefghijklmnopqrstuvwxyz"


def weight_gen():
    if random.random() < 0.6:
        return 1
    else:
        return random.choice([2, 3, 4])


def generate_word(head, tail, size, no_same) -> List[str]:
    if no_same:
        result = set()
        while len(result) < size:
            l = random.choice([0, 1, 1, 2, 2, 3, 20])
            n = head + "".join([random.choice(alphabet) for _ in range(l)]) + tail
            result.add(n)
        return list(result)
    else:
        result = list()
        for _ in range(size):
            l = random.choice([0, 0, 1, 2, 20])
            n = head + "".join([random.choice(alphabet) for _ in range(l)]) + tail
            result.append(n)
        return result


def generate_no_cycle(length):
    arrow = [[0 for _ in range(26)] for _ in range(26)]
    for _ in range(length):
        a = random.randint(0, 24)
        b = random.randint(a + 1, 25)
        if a != b:
            arrow[a][b] += weight_gen()
    return arrow


def generate_has_cycle(length):
    arrow = [[0 for _ in range(26)] for _ in range(26)]
    for _ in range(length):
        a = random.randint(0, 25)
        b = random.randint(0, 25)
        if a != b:
            arrow[a][b] += weight_gen()
    return arrow


def plain_join(words):
    return " ".join(words)


def get_splitter():
    l = random.randint(1, 3)
    return "".join([random.choice("[]-=+ \t!@#$%~\\];,.\n") for _ in range(l)])


def get_confused_word(word):
    return "".join(x.lower() if random.random() < 0.5 else x.upper() for x in word)


def confuse_join(words):
    for i in range(10):
        words.append(alphabet[random.randint(0, 25)])

    random.shuffle(words)
    result = get_splitter()
    for w in words:
        result += get_confused_word(w)
        result += get_splitter()
    return result


def generate(length, no_same, self_cycle_max, no_cycle, confuse):
    """

    :param length: 输入长度
    :param no_same: 不允许完全一样的单词
    :param self_cycle_max: 自环的数量上限
    :param no_cycle: 不允许输入单词环
    :param confuse: 混淆输出
    :return:
    """
    if no_cycle:
        matrix = generate_no_cycle(length)
    else:
        matrix = generate_has_cycle(length)

    if self_cycle_max > 0:
        for i in range(26):
            if random.random() < 0.7:
                matrix[i][i] = random.randint(1, self_cycle_max)

    result = []
    for i, row in enumerate(matrix):
        for j, v in enumerate(row):
            if v > 0:
                result += generate_word(alphabet[i], alphabet[j], v, no_same)

    result = random.sample(result, length)

    if confuse:
        return confuse_join(result)
    else:
        return plain_join(result)


if __name__ == '__main__':
    r = generate(length=1000, no_same=True, self_cycle_max=4, no_cycle=True, confuse=True)
    print(r)
