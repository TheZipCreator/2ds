#define byte unsigned char

typedef struct Point {
    byte x;
    byte y;
} Point;

void pushx_(byte **stack, Point *head, byte data, int line) {
    if(head->x < WIDTH - 1) {
        stack[head->x][head->y] = data;
        head->x++;
    } else {
        error("Error: Stack overflow", line);
    }
}

void pushy_(byte **stack, Point *head, byte data, int line) {
    if(head->y < HEIGHT - 1) {
        stack[head->x][head->y] = data;
        head->y++;
    } else {
        error("Error: Stack overflow", line);
    }
}

byte popx_(byte **stack, Point *head, int line) {
    if(head->x > 0) {
        byte val = stack[head->x-1][head->y];
        head->x--;
        return val;
    } else {
        error("Error: Stack underflow", line);
    }
}

byte popy_(byte **stack, Point *head, int line) {
    if(head->y > 0) {
        byte val = stack[head->x][head->y-1];
        head->y--;
        return val;
    } else {
        error("Error: Stack underflow", line);
    }
}