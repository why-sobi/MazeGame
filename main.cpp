#include <iostream>
#include <ctime>
#include <time.h>
#include <random>
#include <curses/curses.h>

char toUpper(char input) // to convert lower to upper case
{
    if ('A' <= input && input <= 'Z')
        return input;
    else
        return (input - 32);
}

struct Coordinates // row and column respectively
{
    int r, c;
    Coordinates(const int row = -1, const int col = -1) : r(row), c(col) {}

    friend std::ostream &operator<<(std::ostream &out, const Coordinates &obj)
    {
        out << "(" << obj.r << ", " << obj.c << ")";
        return out;
    }
};

template <typename T>
struct OrderNode // node used for the creation of stacks and queues
{
    T data;
    OrderNode *next;

    OrderNode() : next(nullptr) {}
    OrderNode(const T &val) : data(val), next(nullptr) {}
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
    Node(const char val) : data(val) { up = down = left = right = nullptr; }
};
class Grid // Grid for the game
{
    Node *head;
    int size;
    bool hideContent;

    bool placedAt(const int row, const int col, const char ch) // iterates to the given position and places ch if the position is empty
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
        int bombs = (size == 10) ? 6 : (size == 15) ? 10
                                   : (size == 20)   ? 15
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
    void removeCoins() // replaves all coins with default value (for the randomization of coins)
    {
        for (Node *start = head; start; start = start->down)
        {
            for (Node *curr = start; curr; curr = curr->right)
            {
                if (curr->data == 'C')
                    curr->data = '.';
            }
        }
    }
    void deleteGrid()
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
        deleteGrid();
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
        int xcor = 1, ycor = 8;

        for (int i = 0; i < size + 2; i++)
        {
            mvprintw(ycor, xcor, "#");
            xcor += 2;
        }
        ycor++;

        for (Node *start = head; start; start = start->down)
        {
            xcor = 1;
            mvprintw(ycor, xcor, "# ");
            xcor += 2;
            for (Node *curr = start; curr; curr = curr->right)
            {
                (curr->data == 'P' || curr->data == 'B' || curr->data == 'C') ? mvprintw(ycor, xcor, "%c ", curr->data) : hideContent ? mvprintw(ycor, xcor, ". ")
                                                                                            : mvprintw(ycor, xcor, "%c ", curr->data); // print content
                xcor += 2;
            }
            mvprintw(ycor, xcor, "# ");
            ycor++;
        }

        xcor = 1;
        for (int i = 0; i < size + 2; i++)
        {
            mvprintw(ycor, xcor, "# ");
            xcor += 2;
        }
    }
    void setupGame() // set up the game board with the given size (bombs, player, door, coins and key
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
    void randomizeCoins() // removes and then sets up coins again
    {
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

    // operator overload
    void operator=(Grid &copy)
    { // to copy two grids
        this->size = copy.size;

        if (this->head)
            this->deleteGrid();

        Node *prevRowStart = nullptr; // reference to the first node of the last row's first node

        Node *start = copy.head;
        for (int i = 0; i < this->size; i++)
        {
            Node *prev = nullptr;         // reference to the previous node
            Node *prevRowSetup = nullptr; // reference to node above the current node
            Node *curr = start;

            for (int j = 0; j < this->size; j++) {
                if (!this->head)
                {
                    this->head = new Node(curr->data);
                    prev = this->head;
                    prevRowSetup = this->head;
                }
                else
                {
                    Node *newNode = new Node(curr->data);
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
                curr = curr->right;
            }
            prevRowStart = prevRowSetup;
            start = start->down;
        }
    }

};

class Game
{
    Grid grid;                        // grid
    Grid copyOriginal;                // to keep the copy of the original
    Node *player;                     // node reference to the player node
    Stack<char> undo;                 // a stack that withholds the previous move for undo purposes
    Queue<Coordinates> coinCollected; // a queue to preserve the order of the coordinates of the coins collected
    int undoCount;                    // number of undone the player is allowed to do
    int totalMoves;                   // total moves the player has to reach first to the key and then the door
    int score;                        // player's score
    bool hasKey;                      // if the player has acquired the key
    bool atGate;                      // if the player has reached the gate
    bool hasWon;                      // if player has Won

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
    { // simply sets the number of undone allowed based upon the level
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
        default:
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
    bool dataController(const char value) // handles logic for different data in the nodes encountered (returns true if encounters a coin)
    {
        switch (value)
        {
        case 'B':
            mvprintw(5, 1, "Stepped on bomb");
            totalMoves = 1;
            break;
        case 'C':
            mvprintw(5,1, "Collected a coin\n");
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
                mvprintw(5, 1, "Reached the door, you win!") ;
                hasWon = true;
                score += totalMoves;
                totalMoves = 1;
            }
            else
            {
                mvprintw(5, 1, "You need a key to open the door!");
                atGate = true;
            }
        }
        refresh();
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
            lastPos.r++;
            return (calSteps(lastPos, goal) > calSteps(currPos, goal));
        case 'S':
            lastPos.r--;
            return (calSteps(lastPos, goal) > calSteps(currPos, goal));
        case 'A':
            lastPos.c++;
            return (calSteps(lastPos, goal) > calSteps(currPos, goal));
        case 'D':
            lastPos.c--;
            return (calSteps(lastPos, goal) > calSteps(currPos, goal));
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
    Game(const int size) : grid(Grid(size))
    {
        grid.setupGame();
        copyOriginal = grid;
        setPlayerNode();
        totalMoves = calTotalMoves();
        setUndoCount();

        score = 0;
        hasKey = false;
        atGate = false;
        hasWon = false;

        grid.showContent();
    }

    void print() const { grid.print(); }
    void showGrid()
    {
        clear();
        mvprintw(1, 1, "Score: %d", score);
        gotCloser() ? mvprintw(2, 1, "Hint: Getting Closer!") : mvprintw(2, 1, "Hint: Father Away!");
        (hasKey) ? mvprintw(3, 1, "Key Status: Acquired!") : mvprintw(3, 1, "Key Status: Not Acquired!");
        mvprintw(4, 1, "Moves Left: %d", totalMoves);
        mvprintw(4, 20, "Undo Remaining: %d", undoCount);
        mvprintw(5, 1, "Press (W/A/S/D) and Undo(U)");

        print();
        refresh();
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
    void showResult()
    {
        clear();
        mvprintw(1, 1, "Score: %d", score);

        mvprintw(2, 1, "Coins collected at: ");
        for (int i = 22; !coinCollected.isEmpty(); i+=8)
        {
            Coordinates temp = coinCollected.dequeue();
            mvprintw(2, i, "(%d, %d)", temp.r, temp.c);
        }

        mvprintw(3, 1, "Press 0 to exit!");
        (hasWon) ? mvprintw(4, 1, "You Won!") : mvprintw(4, 1, "You Lost!");
        mvprintw(5, 1, "------------------------------------------");
        mvprintw(6, 1, "--------------Original Board--------------");
        mvprintw(7, 1, "------------------------------------------");
        copyOriginal.showContent();

        refresh();

        char c = 0;
        while (c != '0')
        {
            c = getchar();
        }
    }

    void startGame()
    {
        // Initialize the PDCurses screen
        initscr();
        cbreak();
        noecho();
        // keypad(stdscr, TRUE); // Enable special keys like arrows
        curs_set(0); // Hide the cursor
        refresh();

        time_t startTime = time(nullptr);
        time_t currTime;
        const double interval = 10.0;
        showGrid();

        while (totalMoves)
        {
            currTime = time(nullptr);
            if (difftime(currTime, startTime) >= interval)
            {
                updateCoinsLocation();
                startTime = currTime;
            }
            char input;
            std::cin >> input;
            if (input == 'u' || input == 'U')
                undoMove();
            else
            {
                movePlayer(input);
            }
        }
        showResult();
        endwin();
    }
};

int main()
{
    srand(time(NULL));

    int size;
    char input;

    std::cout << "Welcome to the game!" << std::endl;
    std::cout << "Please select a grid size:" << std::endl;
    std::cout << "1. 10x10" << std::endl;
    std::cout << "2. 15x15" << std::endl;
    std::cout << "3. 20x20" << std::endl;
    std::cout << "Enter your choice (1/2/3): ";
    std::cin >> input;

    switch (input)
    {
    case '1':
        size = 10;
        break;
    case '2':
        size = 15;
        break;
    case '3':
        size = 20;
        break;
    default:
        std::cout << "Invalid choice. Defaulting to 15x15 grid." << std::endl;
        size = 15;
    }

    Game game(size);
    game.startGame();

    return 0;
}
