#include <iostream>
#include <random>

char toUpper(char input)
{
    if ('A' <= input && input <= 'Z')
        return input;
    else
        return input - 32;
}

struct Coordinates
{
    int r, c;
    Coordinates(int row = -1, int col = -1) : r(row), c(col) {}
};

template <typename T>
struct OrderNode
{
    T data;
    OrderNode *next;

    OrderNode() : next(nullptr) {}
    OrderNode(const T &val) : next(nullptr), data(val) {}
};

template <typename T>
class Stack
{
    OrderNode<T> *top;
    int size;

public:
    Stack() : size(0) { top = nullptr; }
    ~Stack()
    {
        while (top != nullptr)
        {
            OrderNode<T> *temp = top;
            top = top->next;
            delete temp;
        }
        top = nullptr;
    }

    // methods
    void push(T value)
    {
        if (!top)
            top = new OrderNode<T>(value);
        else
        {
            OrderNode<T> *newNode = new OrderNode<T>(value);
            newNode->next = top;
            top = newNode;
        }
    }
    T peek()
    {
        return top->data;
    }
    T pop()
    {
        T val = top->data;
        OrderNode<T> *temp = top;
        top = top->next;
        delete temp;

        return val;
    }
    bool isEmpty()
    {
        return top == nullptr;
    }
};

template <typename T>
class Queue
{
    OrderNode<T> *start;
    OrderNode<T> *end;

public:
    Queue() : start(nullptr), end(nullptr) {}
    ~Queue() { clear(); }

    void enqueue(const T &data)
    {
        OrderNode<T> *newNode = new OrderNode<T>(data);
        if (!start)
        {
            start = newNode;
            end = newNode;
        }
        else
        {
            end->next = newNode;
            end = newNode;
        }
    }

    T dequeue()
    {
        if (isEmpty())
        {
            return T();
        }
        OrderNode<T> *temp = start;
        T val = start->data;
        start = start->next;
        delete temp;

        return val;
    }

    bool isEmpty() { return !start; }
    void clear()
    {
        while (start)
        {
            OrderNode<T> *temp = start;
            start = start->next;
            delete temp;
        }
        start = end = nullptr;
    }
};

struct Node
{
    char data;
    Node *up, *down, *left, *right;

    Node() { up = down = left = right = nullptr; }
    Node(char val) : data(val) { up = down = left = right = nullptr; }
};
class Grid
{
    Node *head;
    int size;
    bool hideContent;

    bool placedAt(int row, int col, char ch)
    {
        if (!head)
            return false;
        Node *curr = head;
        for (int i = 0; i < row; i++)
            curr = curr->down;
        for (int j = 0; j < col; j++)
            curr = curr->right;

        if (curr->data == '.')
        {
            curr->data = ch;
            return true;
        }
        return false;
    }
    bool emptyNode(int row, int col)
    {
        if (!head)
            return false;
        Node *curr = head;
        for (int i = 0; i < row; i++)
            curr = curr->down;
        for (int j = 0; j < col; j++)
            curr = curr->right;

        return curr->data == '.';
    }
    void setupGrid(int s)
    {
        Node *prevRowStart = nullptr;

        for (int i = 0; i < s; i++)
        {
            Node *prev = nullptr;
            Node *prevRowSetup = nullptr;
            for (int j = 0; j < s; j++)
            {
                if (!head)
                {
                    head = new Node('.');
                    prev = head;
                    prevRowSetup = head;
                }
                else
                {
                    Node *newNode = new Node('.');
                    if (prev)
                    { // passed first column
                        connect_horizontally(prev, newNode);
                        prev = newNode;
                    }
                    if (prevRowStart)
                    { // passed the first row
                        connect_vertically(prevRowStart, newNode);
                        prevRowStart = prevRowStart->right;
                        // std::cout << prevRowStart->data << ' ';
                    }
                    if (j == 0)
                    { // first column
                        prevRowSetup = newNode;
                        prev = newNode;
                    }
                }
            }
            prevRowStart = prevRowSetup;
        }
    }
    void setupPlayer()
    {
        int r, c;
        r = rand() % 14;
        c = rand() % 14;
        placedAt(r, c, 'P');
    }
    void setupKey()
    {
        int r, c;
        bool placed = false;

        while (!placed)
        {
            r = rand() % 14;
            c = rand() % 14;
            placed = placedAt(r, c, 'K');
        }
    }
    void setupDoor()
    {
        int r, c;
        bool placed = false;

        while (!placed)
        {
            r = rand() % 14;
            c = rand() % 14;
            placed = placedAt(r, c, 'D');
        }
    }
    void setupBombs()
    {
        int r, c;
        int bombs = (size == 15) ? 6 : (size == 20) ? 10
                                   : (size == 25)   ? 15
                                                    : 0;

        while (bombs)
        {
            r = rand() % 14;
            c = rand() % 14;
            if (placedAt(r, c, 'B'))
                bombs--;
        }
    }
    void setupCoins()
    {
        int r, c;
        int coins = (size == 15) ? 6 : (size == 20) ? 10
                                   : (size == 25)   ? 15
                                                    : 0;

        while (coins)
        {
            r = rand() % 14;
            c = rand() % 14;
            if (placedAt(r, c, 'C'))
                coins--;
        }
    }

public:
    Grid() : hideContent(true) { head = nullptr; }
    Grid(int s) : size(s), hideContent(true)
    {
        head = nullptr;
    }
    ~Grid()
    {
        Node *nextRow = nullptr;
        Node *nextCol = nullptr;
        for (Node *row = head; row; row = nextRow)
        {
            nextRow = row->down;
            for (Node *col = row; col; col = nextCol)
            {
                nextCol = col->right;
                delete col;
            }
        }
        head = nullptr;
    }

    // methods
    void connect_horizontally(Node *&prev, Node *&follower)
    {
        prev->right = follower;
        follower->left = prev;
    }
    void connect_vertically(Node *&upper, Node *&lower)
    {
        upper->down = lower;
        lower->up = upper;
    }
    void print() const
    {
        for (int i = 0; i < size + 2; i++)
            std::cout << '#' << ' ';
        std::cout << '\n';

        for (Node *start = head; start; start = start->down)
        {
            std::cout << '#' << ' ';
            for (Node *curr = start; curr; curr = curr->right)
            {
                (curr->data == 'P') ? (std::cout << curr->data << ' ') : hideContent ? std::cout << ". "
                                                                                     : std::cout << curr->data << ' '; // print content
            }
            std::cout << '#' << ' ';
            std::cout << '\n';
        }

        for (int i = 0; i < size + 2; i++)
            std::cout << '#' << ' ';
        std::cout << '\n';
    }
    void setupGame()
    {
        // Setup the game board with the given size (bombs, player, door, coins and key)
        setupGrid(size);
        setupPlayer();
        setupKey();
        setupDoor();
        setupBombs();
        setupCoins();
    }
    void showContent()
    {
        hideContent = false;
        print();
        hideContent = true;
    }

    // getters
    Node *getPlayer()
    {
        for (Node *start = head; start; start = start->down)
        {
            for (Node *curr = start; curr; curr = curr->right)
            {
                if (curr->data == 'P')
                    return curr;
            }
        }
        return nullptr;
    }
    Coordinates getCoordinates(char ch)
    {
        int row = 0, col = 0;

        for (Node *start = head; start; start = start->down)
        {
            col = 0;
            for (Node *curr = start; curr; curr = curr->right)
            {
                if (curr->data == ch)
                {
                    return Coordinates(row, col);
                }
                col++;
            }
            row++;
        }

        return Coordinates();
    }
};

class Game
{
    Grid grid;
    Node *player;
    Stack<char> undo;
    Queue<Coordinates> coinCollected;
    int totalMoves;
    bool hasKey;
    bool atGate;

    void setPlayerNode()
    {
        player = grid.getPlayer();
    }
    int calSteps(const Coordinates &start, const Coordinates &stop)
    {
        int steps = (abs(start.r - stop.r) + abs(start.c - stop.c));
        return steps;
    }

    int calTotalMoves()
    {
        int steps = 0;
        Coordinates player = grid.getCoordinates('P');
        Coordinates key = grid.getCoordinates('K');
        Coordinates door = grid.getCoordinates('D');

        steps = (calSteps(player, key) + calSteps(key, door)); // from player to key then to the door
        return steps;
    }
    bool dataController(char value)
    {
        switch (value)
        {
        case 'B':
            std::cout << "Stepped on bomb (quite unfortunate if you ask me)\n";
            totalMoves = 1;
            break;
        case 'C':
            std::cout << "Collected a coin\n";
            return true;
            break;
        case 'K':
            hasKey = true;
            break;
        case 'D':
            if (hasKey)
            {
                std::cout << "Reached the door, you win!\n";
                totalMoves = 1;
            }
            else
            {
                std::cout << "You need a key to open the door!\n";
                atGate = true;
            }
        }
        return false;
    }

    bool moveUp()
    {
        if (player->up)
        {
            player->data = (atGate) ? ('D') : ('.');
            atGate = false;

            bool collected_coin = dataController(player->up->data);

            player = player->up;
            player->data = 'P';

            if (collected_coin)
                coinCollected.enqueue(grid.getCoordinates('P'));

            return true;
        }
        return false;
    }
    bool moveDown()
    {
        if (player->down)
        {
            player->data = (atGate) ? ('D') : ('.');
            atGate = false;

            bool collected_coin = dataController(player->down->data);

            player = player->down;
            player->data = 'P';

            if (collected_coin)
                coinCollected.enqueue(grid.getCoordinates('P'));

            return true;
        }
        return false;
    }
    bool moveLeft()
    {
        if (player->left)
        {
            player->data = (atGate) ? ('D') : ('.');
            atGate = false;

            bool collected_coin = dataController(player->left->data);

            player = player->left;
            player->data = 'P';

            if (collected_coin)
                coinCollected.enqueue(grid.getCoordinates('P'));
            return true;
        }
        return false;
    }
    bool moveRight()
    {
        if (player->right)
        {
            player->data = (atGate) ? ('D') : ('.');
            atGate = false;

            bool collected_coin = dataController(player->right->data);

            player = player->right;
            player->data = 'P';

            if (collected_coin)
                coinCollected.enqueue(grid.getCoordinates('P'));
            return true;
        }
        return false;
    }

public:
    Game(int size) : grid(Grid(size))
    {
        grid.setupGame();
        setPlayerNode();
        totalMoves = calTotalMoves();
    }

    void print() const { grid.print(); }
    void showGrid()
    {
        system("CLS"); // clear the terminal
        grid.showContent();
    }

    void movePlayer(char input)
    {
        bool moved = false;

        switch (input)
        {
        case 'W':
        case 'w':
            if (undo.isEmpty() || undo.peek() != 'S')
            {
                moved = moveUp();
            }
            break;
        case 'A':
        case 'a':
            if (undo.isEmpty() || undo.peek() != 'D')
            {
                moved = moveLeft();
            }
            break;
        case 'S':
        case 's':
            if (undo.isEmpty() || undo.peek() != 'W')
            {
                moved = moveDown();
            }
            break;
        case 'D':
        case 'd':
            if (undo.isEmpty() || undo.peek() != 'A')
            {
                moved = moveRight();
            }
            break;
        }
        if (moved)
        {
            undo.push(toUpper(input));
            totalMoves--;
            showGrid();
        }
        std::cout << "Moves left: " << totalMoves << '\n';
    }

    void undoMove()
    {
        if (undo.isEmpty())
        {
            showGrid();
            return;
        }
        char move = undo.pop();
        switch (move)
        {
        case 'W':
            moveDown();
            break;
        case 'S':
            moveUp();
            break;
        case 'D':
            moveLeft();
            break;
        case 'A':
            moveRight();
            break;
        }
        totalMoves++;
        showGrid();
    }
    int getPlayerMoves()
    {
        return totalMoves;
    }
};
