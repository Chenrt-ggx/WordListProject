import re
from typing import List


def split_word(content: bytes) -> List[bytes]:
    pattern = re.compile(b"[a-zA-Z]{2,}")
    return [x.lower() for x in pattern.findall(content)]


def to_ascii(content: bytes) -> bytes:
    # make every bytes decodable
    ba = bytearray(content)
    for i, v in enumerate(ba):
        if v >= 128:
            ba[i] = ord('.')
    return bytes(ba)


if __name__ == '__main__':
    ans = split_word(b"he a-xxpaAjowg\nahwiu")
    print(ans)
