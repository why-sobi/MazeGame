#include <iostream>
#include <random>

char toUpper(char input) // to convert lower to upper case
{
    if ('A' <= input && input <= 'Z')
        return input;
    else
        return input - 32;
}

struct Coordinates // row and coloumn respectively
{
    int r, c;
    Coordinates(int row = -1, int col = -1) : r(row), c(col) {}
};

template <typename T>
struct OrderNode // node used for the creation of stacks and queues
{
    T data;
    OrderNode *next;

    OrderNode() : next(nullptr) {}
    OrderNode(const T &val) : next(nullptr), data(val) {}
};

template <typename T> // basic template for stacks
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

template <typename T> // basic template for queue
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

struct Node // node structure for the grid
{
    char data;
    Node *up, *down, *left, *right;

    Node() { up = down = left = right = nullptr; }
    Node(char val) : data(val) { up = down = left = right = nullptr; }
};
class Grid // Grid for the game
{
    Node *head;
    int size;
    bool hideContent;

    bool placedAt(int row, int col, char ch) // iterates to the given position and places ch if the position is empty
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
    bool emptyNode(int row, int col) // tells if the data for the specified node is has default value
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
    void setupGrid(int s) // sets up the grid by connecting all the nodes
    {
        Node *prevRowStart = nullptr; // reference to the first node of the last row's first node

        for (int i = 0; i < s; i++)
        {
            Node *prev = nullptr;         // reference to the previous node
            Node *prevRowSetup = nullptr; // reference to node above the current node
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
    void setupPlayer() // gives random node the value of the player
    {
        int r, c;
        r = rand() % size;
        c = rand() % size;
        placedAt(r, c, 'P');
    }
    void setupKey() // gives random coordinates to the key
    {
        int r, c;
        bool placed = false;

        while (!placed)
        {
            r = rand() % size;
            c = rand() % size;
            placed = placedAt(r, c, 'K'); // returns true if the key has been placed on a node with the default value
        }
    }
    void setupDoor() // gives random coordinates to the door
    {
        int r, c;
        bool placed = false;

        while (!placed)
        {
            r = rand() % size;
            c = rand() % size;
            placed = placedAt(r, c, 'D');
        }
    }
    void setupBombs() // gives random coordinates to random bombs until all have been placed at an empty node
    {
        int r, c;
        int bombs = (size == 15) ? 6 : (size == 20) ? 10
                                   : (size == 25)   ? 15
                                                    : 0;

        while (bombs)
        {
            r = rand() % size;
            c = rand() % size;
            if (placedAt(r, c, 'B'))
                bombs--;
        }
    }
    void setupCoins() // gives random coordinates to random coins until all have been placed at an empty node
    {
        int r, c;
        int coins = (size == 10) ? 6 : (size == 15) ? 10
                                   : (size == 20)   ? 15
                                                    : 0;

        while (coins)
        {
            r = rand() % size;
            c = rand() % size;
            if (placedAt(r, c, 'C'))
                coins--;
        }
    }
    void removeCoins()
    { // replaves all coins with default value (for the randomization of coins)
        for (Node *start = head; start; start = start->down)
        {
            for (Node *curr = start; curr; curr = curr->right)
            {
                if (curr->data == 'C')
                    curr->data = '.';
            }
        }
    }

public:
    // constructors
    Grid() : hideContent(true) { head = nullptr; }
    Grid(int s) : size(s), hideContent(true)
    {
        head = nullptr;
    }
    // Destructors
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
    void connect_horizontally(Node *&prev, Node *&follower) // two nodes to connect their left and right pointers
    {
        prev->right = follower;
        follower->left = prev;
    }
    void connect_vertically(Node *&upper, Node *&lower) // two nodes to connect their upper and lower pointers
    {
        upper->down = lower;
        lower->up = upper;
    }
    void print() const // simply prints the grid
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
    void setupGame() // setup the game board with the given size (bombs, player, door, coins and key
    {
        setupGrid(size);
        setupPlayer();
        setupKey();
        setupDoor();
        setupBombs();
        setupCoins();
    }
    void showContent() // to show all the contents of the board
    {
        hideContent = false;
        print();
        hideContent = true;
    }
    void randomizeCoins()
    { // removes and then sets up coins again
        removeCoins();
        setupCoins();
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
    Coordinates getCoordinates(char ch) // returns the row and column of the firs appearance of the specified data in the grid
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
    int getSize() const { return size; }
};

class Game
{
    Grid grid;                        // grid
    Node *player;                     // node reference to the player node
    Stack<char> undo;                 // a stack that witholds the previous move for undo purposes
    Queue<Coordinates> coinCollected; // a queue to preserve the order of the coordinates of the coins collected
    int undoCount;                    // number of undos the player is allowed to do
    int totalMoves;                   // total moves the player has to reach first to the key and then the door
    int score;                        // player's score
    bool hasKey;                      // if the player has accquired the key
    bool atGate;                      // if the player has reached the gate

    void setPlayerNode() // sets the reference to the player node
    {
        player = grid.getPlayer();
    }
    int calSteps(const Coordinates &start, const Coordinates &stop) // calculates steps required to move from the starting coordinates to the ending ones
    {
        int steps = (abs(start.r - stop.r) + abs(start.c - stop.c));
        return steps;
    }
    void setUndoCount()
    { // simply sets the number of undos allowed based upon the level
        switch (grid.getSize())
        {
        case 10:
            this->undoCount = 6;
            break;
        case 15:
            this->undoCount = 4;
            break;
        case 20:
            this->undoCount = 1;
            break;
        }
    }

    int calTotalMoves() // calculates the total moves required for player to reach key and then to door
    {
        int steps = 0;
        Coordinates player = grid.getCoordinates('P');
        Coordinates key = grid.getCoordinates('K');
        Coordinates door = grid.getCoordinates('D');

        steps = (calSteps(player, key) + calSteps(key, door)); // from player to key then to the door

        switch (grid.getSize())
        { // increases the allowed move if the difficulty is either easy or meduim
        case 10:
            steps += 6;
            break;
        case 15:
            steps += 2;
            break;
        }

        return steps;
    }
    bool dataController(char value) // handles logic for different data in the nodes encountered (returns true if encounters a coin)
    {
        switch (value)
        {
        case 'B':
            std::cout << "Stepped on bomb (quite unfortunate if you ask me)\n";
            totalMoves = 1;
            break;
        case 'C':
            std::cout << "Collected a coin\n";
            undoCount++;
            score += 2;
            return true;
            break;
        case 'K':
            hasKey = true;
            break;
        case 'D':
            if (hasKey)
            {
                std::cout << "Reached the door, you win!\n";
                score += totalMoves;
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
    bool gotCloser() // returns true if the current move got him closer to his current goal
    {
        if (undo.isEmpty())
            return false;
        char lastMove = undo.peek();
        Coordinates currPos = grid.getCoordinates('P');
        Coordinates lastPos = currPos;

        Coordinates goal = (hasKey) ? grid.getCoordinates('D') : grid.getCoordinates('K');

        switch (lastMove)
        {
        case 'W':
            lastPos.r--;
            return (calSteps(lastPos, goal) < calSteps(lastPos, goal));
        case 'S':
            lastPos.r++;
            return (calSteps(lastPos, goal) < calSteps(lastPos, goal));
        case 'A':
            lastPos.c++;
            return (calSteps(lastPos, goal) < calSteps(lastPos, goal));
        case 'D':
            lastPos.c--;
            return (calSteps(lastPos, goal) < calSteps(lastPos, goal));
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

        setUndoCount();
        score = 0;
        hasKey = false;
        grid.showContent();
    }

    void print() const { grid.print(); }
    void showGrid()
    {
        system("CLS"); // clear the terminal

        std::cout << "Score: " << score << '\t' << "Hint: ";
        gotCloser() ? std::cout << "Getting Closer!\n" : std::cout << "Getting Father Away!\n";
        std::cout << "Moves left: " << totalMoves << '\n';

        std::cout << "\n\n";
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
    }

    void undoMove()
    {
        if (undo.isEmpty() || !undoCount)
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
        undoCount--;
        showGrid();
    }
    int getPlayerMoves()
    {
        return totalMoves;
    }
    void updateCoinsLocation()
    {
        grid.randomizeCoins();
    }
};
