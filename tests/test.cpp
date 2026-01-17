#include <iostream>
#include <string>
using namespace std;
// Logical position (NOT byte index)
struct Position {
    size_t index;
};
class TextBuffer {
public:
    TextBuffer(string text) : data(move(text)) {}

    // Return UTF-8 character as string
    string char_at(size_t logical_index) const {
        size_t start = utf8_byte_offset(logical_index);
        size_t end   = utf8_byte_offset(logical_index + 1);
        return data.substr(start, end - start);
    }

    size_t length() const {
        size_t count = 0;
        for (size_t i = 0; i < data.size();) {
            unsigned char c = data[i];
            if ((c & 0x80) == 0) i += 1;
            else if ((c & 0xE0) == 0xC0) i += 2;
            else if ((c & 0xF0) == 0xE0) i += 3;
            else if ((c & 0xF8) == 0xF0) i += 4;
            count++;
        }
        return count;
    }

private:
    string data;

    size_t utf8_byte_offset(size_t logical_index) const {
        size_t byte = 0;
        size_t count = 0;

        while (byte < data.size() && count < logical_index) {
            unsigned char c = data[byte];
            if ((c & 0x80) == 0) byte += 1;
            else if ((c & 0xE0) == 0xC0) byte += 2;
            else if ((c & 0xF0) == 0xE0) byte += 3;
            else if ((c & 0xF8) == 0xF0) byte += 4;
            count++;
        }
        return byte;
    }
};

void move_right(Position& pos, const TextBuffer& text) {
    if (pos.index + 1 < text.length()) {
        pos.index++;
    }
}


int main() {
    TextBuffer text(u8"héllo");
    Position cursor{0};

    cout << "Text length: " << text.length() << "\n";

    cout << "Cursor at: " << text.char_at(cursor.index) << "\n";
     move_right(cursor, text);
    cout << "Cursor moved to: " << text.char_at(cursor.index) << "\n";

    move_right(cursor, text);
    cout << "Cursor moved to: " << text.char_at(cursor.index) << "\n";
    move_right(cursor, text);
    cout << "Cursor moved to: " << text.char_at(cursor.index) << "\n";

    move_right(cursor, text);
    cout << "Cursor moved to: " << text.char_at(cursor.index) << "\n";



    return 0;
}
