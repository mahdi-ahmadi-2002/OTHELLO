#include <iostream>
#include <SFML/Graphics.hpp>
#include<WS2tcpip.h>
#include <thread>
#include <winsock2.h>
#include <windows.h>
#include <string>
#include <fstream>
#pragma comment (lib, "Ws2_32.lib")
using namespace std;
using namespace sf;
#define BUFFERLEN 3
#define PORT 1003
SOCKET NewClient;
Font font;
bool is_possible(char ***arr, int size, int row, int column, int round, char turn);
void swap(char ***arr, int size, int row, int column, int round, char turn);
void main_window(int n, char ***arr, int work, int menu_num, string name1, string name2);
void ai_nums(char ***arr, int size,int &row, int &column, int round, char turn);
void define_server(int n);
void send(int x, int y);
int receive(int &x, int &y);
int menu();
int choose_game();
void Score(int n, char ***arr,int &x, int &o, int turn, int round);
int size();
void start();
void winner_fun(int x_num, int o_num, string name1, string name2);
void get_names(string &name1, string &name2);
void save(char ***arr, int n, string &name_1, string &name_2, int *turn, int round);
void Rankings();
void counter(string *arr, int n);
void ranking_add(string str);
int main()
{
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        cerr << "font was not found. exiting ...";
        Sleep(2000);
        exit(1);
    }
    start();
    return 0;
}

void start(){
    string name1, name2;
    int menu_num = menu();
    if(menu_num == 1) {
        int work = choose_game();
        int n = size();
        char ***arr = new char **[n];
        for (int i = 0; i < n; ++i) {
            arr[i] = new char *[n];
            for (int j = 0; j < n; ++j) {
                arr[i][j] = new char[n * n - 4];
            }
        }
        if(work == 1){
            get_names(name1, name2);
        }
        if(work == 2){
            name1 = "Computer";
            name2 = "you";
        }

        if(work == 3){
            name1 = "opponent";
            name2 = "you";
        }
        main_window(n, arr, work,menu_num, name1, name2);
    }else if(menu_num == 2){
        string temp;
        int n;
        ifstream read("C:/Users/1381m/OneDrive/Desktop/New folder/HelloSFML/cmake-build-debug/Loading_game.txt");
        while (getline (read, temp)) {
            n = stoi(temp);
            break;
        }
        char ***arr = new char**[n];
        for (int i = 0; i < n; ++i) {
            arr[i] = new char*[n];
            for (int j = 0; j < n; ++j) {
                arr[i][j] = new char[n * n - 4];
            }
        }

        main_window(n, arr, 1, menu_num, name1, name2);
        read.close();
    }else{
        Rankings();
    }
}

void get_names(string &name1, string &name2){
    RenderWindow window(VideoMode(1388, 900), "Welcome to othello");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyPressed)
            {
                if(event.key.code == Keyboard::Enter){
                    window.close();
                }
            }
            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode > 64 && event.text.unicode < 124) {
                    name1 = name1 + event.text.unicode;
                }
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(Color::Cyan);
        Text name1_text;
        name1_text.setFont(font);
        name1_text.setCharacterSize(50);
        name1_text.setColor(Color::Yellow);
        name1_text.setString(name1);
        name1_text.setPosition(0, 0);
        window.draw(name1_text);
        window.display();
    }
    RenderWindow window1(VideoMode(1388, 900), "Welcome to othello");

    // run the program as long as the window is open
    while (window1.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        Event event;
        while (window1.pollEvent(event))
        {
            if (event.type == Event::KeyPressed)
            {
                if(event.key.code == Keyboard::Enter){
                    window1.close();
                }
            }
            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode > 64 && event.text.unicode < 124) {
                    name2 = name2 + event.text.unicode;
                }
            }
            if (event.type == sf::Event::Closed)
                window1.close();
        }
        window1.clear(Color::Cyan);
        Text name2_text;
        name2_text.setFont(font);
        name2_text.setCharacterSize(50);
        name2_text.setColor(Color::Yellow);
        name2_text.setString(name2);
        name2_text.setPosition(0, 0);
        window1.draw(name2_text);
        window1.display();
    }

}

void main_window(int n, char ***arr, int work, int menu_num, string name1, string name2){
    if(work == 3){
        define_server(n);
    }
    float image_first_x = 0;
    float image_first_y = 0;

    if(work == 3){
     Sleep(10000);
    }
    RenderWindow window(sf::VideoMode(1000, 1000, 32), "SFML Graphics", Style::Close);

    int x_num = 0, o_num = 0;
    float width = 4 * window.getSize().x / 5;
    float height = 4 * window.getSize().y / 5;

    Event event;

    // Rendering white circle
    Texture texture_white_circle;
    texture_white_circle.loadFromFile("C:/Users/1381m/OneDrive/Desktop/Codes/othello_graphic/Resources/white_circle.png");// full directory address
    Sprite sprite_white_circle(texture_white_circle);
    sprite_white_circle.setScale((12.0 / n) * 2.5 * (width / 1600.0), (12.0 / n) * 2.5 * (height / 1600.0));
    sprite_white_circle.setPosition(image_first_x + 5, image_first_y + 5);
    //Rendering black circle
    Texture texture_black_circle;
    texture_black_circle.loadFromFile("C:/Users/1381m/OneDrive/Desktop/Codes/othello_graphic/Resources/black_circle.png");// full directory address
    Sprite sprite_black_circle(texture_black_circle);
    sprite_black_circle.setScale((12.0 / n) * 2.5 * (width / 1600.0), (12.0 / n) * 2.5 * (height / 1600.0));
    sprite_black_circle.setPosition(image_first_x + 5, image_first_y + 5);

    bool run = true;
    int* turn = new int[n * n - 4];
    turn[0] = 0;
    int round = 0;
    if(menu_num == 1) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < n * n - 4; ++k) {
                    arr[i][j][k] = ' ';
                }
            }
        }

        arr[n / 2 - 1][n / 2 - 1][0] = 'X';
        arr[n / 2][n / 2][0] = 'X';
        arr[n / 2][n / 2 - 1][0] = 'O';
        arr[n / 2 - 1][n / 2][0] = 'O';
    }else if(menu_num == 2){
        int counter = 0, i2 = 0, j2 = 0;
        string temp_1;
        char temp;
        string round_form_save;
        ifstream Read_Loadings("Loading_game.txt"); // Reading From Loading_game.txt
        getline(Read_Loadings, temp_1);// ignoring first line
        while (Read_Loadings.get(temp)){
            if (temp == ' '){
                break;
            }
            round_form_save.append(1, temp);
        }
        turn[round] = stoi(round_form_save);
        while (Read_Loadings.get(temp)){
            if (temp == ' '){
                break;
            }
            name1.append(1, temp);
        }
        // getline(Read_Loadings, temp_1);// ignoring third line
        while (Read_Loadings.get(temp)){
            if (temp == ' '){
                break;
            }
            name2.append(1, temp);
        }
        getline(Read_Loadings, temp_1);// ignoring fourth line
        while (Read_Loadings.get(temp)) { // Filling arr and arr_bool
            if (counter >= n){
                counter = counter - n;
                i2++;
                j2 = 0;
            }
            arr[i2][j2][round] = temp;
            j2++;
            counter++;
        }
        Read_Loadings.close();
    }
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            window.clear(Color(50, 100, 50));

            // drawing pieces
            float x1 = 0, y1 = 0, x2 = 0, y2 = height;
            // Vertical lines
            for (int i = 0; i < n + 1; i++) {
                sf::Vertex line_vertical[] = {
                        sf::Vertex(sf::Vector2f(x1, y1)),
                        sf::Vertex(sf::Vector2f(x2, y2))
                };

                window.draw(line_vertical, 2, sf::Lines);

                x1 += width / n;
                x2 += width / n;
            }

            // Horizontal lines
            x1 = 0, y1 = 0, x2 = width, y2 = 0;
            // Vertical lines
            for (int i = 0; i < n + 1; i++) {
                sf::Vertex line_horizontal[] = {
                        Vertex(sf::Vector2f(x1, y1)),
                        Vertex(sf::Vector2f(x2, y2))
                };

                window.draw(line_horizontal, 2, sf::Lines);

                y1 += height / n;
                y2 += height / n;
            }

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
//                    sprite_square.setPosition(image_first_x, image_first_y);
//                    window.draw(sprite_square);

                    // White --> X and Black --> O
                    if (arr[i][j][round] == 'X') {
                        sprite_white_circle.setPosition((width / n) * j, (height / n) * i);
                        window.draw(sprite_white_circle);
                    } else if (arr[i][j][round] == 'O') {
                        sprite_black_circle.setPosition((width / n) * j, (height / n) * i);
                        window.draw(sprite_black_circle);
                    }
                    image_first_x += 120;
                }
                image_first_x = 0;
                image_first_y += 120;
            }

            image_first_x = 0;
            image_first_y = 0;

//                // showing circles (not clicked)
            if (event.type == Event::MouseMoved) {
                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < n; j++) {
                        if (event.mouseMove.y > (height / n) * i && event.mouseMove.y<(height / n) * (i + 1) &&
                                                                                      event.mouseMove.x>(
                                width / n) * j && event.mouseMove.x < (width / n) * (j + 1)) {
                            if (turn[round] % 2 == 0) {
                                sprite_white_circle.setPosition((width / n) * j, (width / n) * i);
                                window.draw(sprite_white_circle);
                            } else {
                                sprite_black_circle.setPosition((width / n) * j, (width / n) * i);
                                window.draw(sprite_black_circle);
                            }
                        }
                    }
                }
            }
            char turnch;
            if (turn[round] % 2 == 1) {
                turnch = 'O';
            } else {
                turnch = 'X';
            }
            bool a1 = work == 2 && turnch == 'O';
            bool a2 = work == 3 && turnch == 'O';
            if (work == 1 || a1 || a2) {
                // submitting circles: 'p' in cmd mode
                int x = 100 , y=100 ;
                if (event.type == sf::Event::TextEntered){

                    if (event.text.unicode == 118){
                        save(arr, n, name1, name2, turn, round);
                        cout << "SAVED!";
                    }else if(event.text.unicode == 117){
                        round -= work;
                    }
                }
                if (event.type == sf::Event::MouseButtonPressed) {

                    if (event.mouseButton.button == sf::Mouse::Left) {
                        for (int i = 0; i < n; i++) {
                            for (int j = 0; j < n; j++) {
                                if (event.mouseButton.y > (height / n) * i &&
                                    event.mouseButton.y<(height / n) * (i + 1) &&
                                    event.mouseButton.x>(width / n) * j &&
                                    event.mouseButton.x < (width / n) * (j + 1)) {

                                    x = i;
                                    y = j;
                                    if ((!is_possible(arr, n, x, y, round, turnch))) {
                                        break;
                                    } else {
                                        round++;
                                        for (int i1 = 0; i1 < n; ++i1) {
                                            for (int j1 = 0; j1 < n; ++j1) {
                                                arr[i1][j1][round] = arr[i1][j1][round - 1];
                                            }
                                        }
                                        swap(arr, n, x, y, round, turnch);
                                        turn[round] = turn[round - 1] + 1;
                                        if (turn[round] % 2 == 1) {
                                            turnch = 'O';
                                        } else {
                                            turnch = 'X';
                                        }
                                        bool flag = false;
                                        for (int i1 = 0; i1 < n; ++i1) {
                                            for (int j1 = 0; j1 < n; ++j1) {
                                                if (is_possible(arr, n, i1, j1, round, turnch)) {
                                                    flag = true;
                                                    break;
                                                }
                                            }
                                        }
                                        if(work == 3){
                                            send(x, y);
                                        }
                                        bool flag_finish = false;
                                        if (!flag) {
                                            turn[round]--;
                                            if (turn[round] % 2 == 1) {
                                                turnch = 'O';
                                            } else {
                                                turnch = 'X';
                                            }
                                            for (int i1 = 0; i1 < n; ++i1) {
                                                for (int j1 = 0; j1 < n; ++j1) {
                                                    if (is_possible(arr, n, i1, j1, round, turnch)) {
                                                        flag_finish = true;
                                                        break;
                                                    }
                                                }
                                            }
                                            if (flag_finish) {
                                                Sleep(5000);
                                                window.close();
                                                winner_fun(x_num, o_num, name1, name2);
                                            }
                                        }
                                    }

                                }
                            }
                        }
                    }
                }
            } else if (work == 2) {
                int x, y;
                ai_nums(arr, n, x, y, round, turnch);
                round++;
                for (int i1 = 0; i1 < n; ++i1) {
                    for (int j1 = 0; j1 < n; ++j1) {
                        arr[i1][j1][round] = arr[i1][j1][round - 1];
                    }
                }
                swap(arr, n, x, y, round, turnch);
                turn[round] = turn[round - 1] + 1;
                if (turn[round] % 2 == 1) {
                    turnch = 'O';
                } else {
                    turnch = 'X';
                }
                bool flag = false;
                for (int i1 = 0; i1 < n; ++i1) {
                    for (int j1 = 0; j1 < n; ++j1) {
                        if (is_possible(arr, n, i1, j1, round, turnch)) {
                            flag = true;
                            break;
                        }
                    }
                }
                bool flag_finish = false;
                if (!flag) {
                    turn[round]--;
                    if (turn[round] % 2 == 1) {
                        turnch = 'O';
                    } else {
                        turnch = 'X';
                    }
                    for (int i1 = 0; i1 < n; ++i1) {
                        for (int j1 = 0; j1 < n; ++j1) {
                            if (is_possible(arr, n, i1, j1, round, turnch)) {
                                flag_finish = true;
                                break;
                            }
                        }
                    }

                    if (flag_finish) {
                        Sleep(5000);
                        window.close();
                        winner_fun(x_num, o_num, name1, name2);//finishes the game before table is completely full
                    }
                }
            }else {
                int x, y;
                receive(x, y);
                round++;
                for (int i1 = 0; i1 < n; ++i1) {
                    for (int j1 = 0; j1 < n; ++j1) {
                        arr[i1][j1][round] = arr[i1][j1][round - 1];
                    }
                }
                swap(arr, n, x, y, round, turnch);
                turn[round] = turn[round - 1] + 1;
                if (turn[round] % 2 == 1) {
                    turnch = 'O';
                } else {
                    turnch = 'X';
                }
                bool flag = false;
                for (int i1 = 0; i1 < n; ++i1) {
                    for (int j1 = 0; j1 < n; ++j1) {
                        if (is_possible(arr, n, i1, j1, round, turnch)) {
                            flag = true;
                            break;
                        }
                    }
                }
                bool flag_finish = false;
                if (!flag) {
                    turn[round]--;
                    if (turn[round] % 2 == 1) {
                        turnch = 'O';
                    } else {
                        turnch = 'X';
                    }
                    for (int i1 = 0; i1 < n; ++i1) {
                        for (int j1 = 0; j1 < n; ++j1) {
                            if (is_possible(arr, n, i1, j1, round, turnch)) {
                                flag_finish = true;
                                break;
                            }
                        }
                    }

                    if (flag_finish) {
                        Sleep(5000);
                        window.close();
                        winner_fun(x_num, o_num, name1, name2);//finishes the game before table is completely full
                    }
                }
            }

            string name1_and_score1 = name1 + " : " + to_string(x_num), name2_and_score2 = name2 + " : " + to_string(o_num) + "\nundo : u\nsave : v";
            Text scores, name_1_score1, name_2_score2;
            Score(n, arr, x_num, o_num, turn[round], round);
            scores.setFont(font);
            scores.setCharacterSize(50);
            scores.setColor(Color::Red);
            scores.setString("Scores");
            scores.setPosition(4.1 * window.getSize().x / 5, window.getSize().y / 18);
            Sleep(1);
            name_1_score1.setFont(font);
            name_1_score1.setCharacterSize(30);
            name_1_score1.setColor(Color::Red);
            name_1_score1.setString(name1_and_score1);
            name_1_score1.setPosition(4.1 * window.getSize().x / 5, window.getSize().y / 5);

            name_2_score2.setFont(font);
            name_2_score2.setCharacterSize(30);
            name_2_score2.setColor(Color::Red);
            name_2_score2.setString(name2_and_score2);
            name_2_score2.setPosition(4.1 * window.getSize().x / 5, window.getSize().y / 4);
            window.draw(scores);
            window.draw(name_1_score1);
            window.draw(name_2_score2);
            window.display();
            if(round == n * n - 4){
                Sleep(5000);
                window.close();
                winner_fun(x_num, o_num, name1, name2);
            }
        }
    }
    start();
}

void save(char ***arr, int n, string &name_1, string &name_2, int *turn, int round){
    ofstream write("Loading_game.txt", ios::out);
    write << n << endl;
    write << turn[round] << " " << name_1 << " " << name_2 << " " << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            write << arr[i][j][round];
        }
    }
    write.close();
}

void winner_fun(int x_num, int o_num, string name1, string name2){
    RenderWindow window1(VideoMode(1066, 940), "Welcome to othello");
    Texture texture_winner;
    texture_winner.loadFromFile("C:/Users/1381m/OneDrive/Desktop/Codes/othello_graphic/Resources/winner.png");// full directory address
    Sprite sprite_winner(texture_winner);
    sprite_winner.setScale(1, 1);
    sprite_winner.setPosition(0, 0);
    Event event;
    string winner;
    if(x_num > o_num){
        winner = "Winner is : "+ name1;
        ranking_add(name1);
    }else if(x_num < o_num){
        winner = "Winner is : "+ name2;
        ranking_add(name2);
    }else{
        winner = "Draw!";
    }
    Text winner_is;
    winner_is.setFont(font);
    winner_is.setCharacterSize(30);
    winner_is.setColor(Color::Yellow);
    winner_is.setString(winner);
    winner_is.setPosition(220, 870);
    while (window1.isOpen()) {
        while (window1.pollEvent(event))
        {
            if (event.type == Event::Closed) {
                window1.close();
            }
        }
        window1.draw(sprite_winner);
        window1.draw(winner_is);
        window1.display();
    }
}

bool is_possible(char ***arr, int size, int row, int column, int round, char turn) {
    if (arr[row][column][round] != ' ') {
        return false;
    }
    int temp_row = row + 2;
    int temp_column = column;
    while (temp_row < size && temp_row >= 0 && arr[row + 1][column][round] != ' ' && arr[row + 1][column][round] != turn && arr[temp_row][column][round] != ' ') {
        if (arr[temp_row][column][round] == turn) {
            return true;
        }
        temp_row++;
    }
    temp_row = row - 2;
    while (temp_row < size && temp_row >= 0 && arr[row - 1][column][round] != ' ' && arr[row - 1][column][round] != turn && arr[temp_row][column][round] != ' ') {
        if (arr[temp_row][column][round] == turn) {
            return true;
        }
        temp_row--;
    }
    temp_column = column + 2;
    while (temp_column < size && temp_column >= 0 && arr[row][column + 1][round] != ' ' && arr[row][column + 1][round] != turn && arr[row][temp_column][round] != ' ') {
        if (arr[row][temp_column][round] == turn) {
            return true;
        }
        temp_column++;
    }
    temp_column = column - 2;
    while (temp_column < size && temp_column >= 0 && arr[row][column - 1][round] != ' ' && arr[row][column - 1][round] != turn && arr[row][temp_column][round] != ' ') {
        if (arr[row][temp_column][round] == turn) {
            return true;
        }
        temp_column--;
    }
    temp_row = row + 2;
    temp_column = column + 2;
    while (temp_column < size && temp_column >= 0 && temp_row < size && temp_row >= 0 && arr[row + 1][column + 1][round] != ' ' && arr[row + 1][column + 1][round] != turn && arr[temp_row][temp_column][round] != ' ') {
        if (arr[temp_row][temp_column][round] == turn) {
            return true;
        }
        temp_row++;
        temp_column++;
    }
    temp_row = row - 2;
    temp_column = column - 2;
    while (temp_column < size && temp_column >= 0 && temp_row < size && temp_row >= 0 && arr[row - 1][column - 1][round] != ' ' && arr[row - 1][column - 1][round] != turn && arr[temp_row][temp_column][round] != ' ') {
        if (arr[temp_row][temp_column][round] == turn) {
            return true;
        }
        temp_row--;
        temp_column--;
    }
    temp_row = row - 2;
    temp_column = column + 2;
    while (temp_column < size && temp_column >= 0 && temp_row < size && temp_row >= 0 && arr[row - 1][column + 1][round] != ' ' && arr[row - 1][column + 1][round] != turn && arr[temp_row][temp_column][round] != ' ') {
        if (arr[temp_row][temp_column][round] == turn) {
            return true;
        }
        temp_row--;
        temp_column++;
    }
    temp_row = row + 2;
    temp_column = column - 2;
    while (temp_column < size && temp_column >= 0 && temp_row < size && temp_row >= 0 && arr[row + 1][column - 1][round] != ' ' && arr[row + 1][column - 1][round] != turn && arr[temp_row][temp_column][round] != ' ') {
        if (arr[temp_row][temp_column][round] == turn) {
            return true;
        }
        temp_row++;
        temp_column--;
    }
    return false;
}

void swap(char ***arr, int size, int row, int column, int round, char turn) {
    arr[row][column][round] = turn;
    bool is_changable = false;
    int temp_row = row + 2;
    int temp_column = column;
    while (temp_row < size && temp_row >= 0 && arr[row + 1][column][round] != ' ' && arr[row + 1][column][round] != turn && arr[temp_row][column][round] != ' ') {
        if (arr[temp_row][column][round] == turn) {
            is_changable = true;
            break;
        }
        temp_row++;
    }
    if (is_changable) {
        for (int i = row + 1; i < temp_row; i++) {
            arr[i][column][round] = turn;
        }
    }
    is_changable = false;
    temp_row = row - 2;
    while (temp_row < size && temp_row >= 0 && arr[row - 1][column][round] != ' ' && arr[row - 1][column][round] != turn && arr[temp_row][column][round] != ' ') {
        if (arr[temp_row][column][round] == turn) {
            is_changable = true;
            break;
        }
        temp_row--;
    }
    if (is_changable) {
        for (int i = row - 1; i > temp_row; i--) {
            arr[i][column][round] = turn;
        }
    }
    is_changable = false;
    temp_column = column + 2;
    while (temp_column < size && temp_column >= 0 && arr[row][column + 1][round] != ' ' && arr[row][column + 1][round] != turn && arr[row][temp_column][round] != ' ') {
        if (arr[row][temp_column][round] == turn) {
            is_changable = true;
            break;
        }
        temp_column++;
    }
    if (is_changable) {
        for (int i = column + 1; i < temp_column; i++) {
            arr[row][i][round] = turn;
        }
    }
    is_changable = false;
    temp_column = column - 2;
    while (temp_column < size && temp_column >= 0 && arr[row][column - 1][round] != ' ' && arr[row][column - 1][round] != turn && arr[row][temp_column][round] != ' ') {
        if (arr[row][temp_column][round] == turn) {
            is_changable = true;
            break;
        }
        temp_column--;
    }
    if (is_changable) {
        for (int i = column - 1; i > temp_column; i--) {
            arr[row][i][round] = turn;
        }
    }
    is_changable = false;
    temp_row = row + 2;
    temp_column = column + 2;
    while (temp_column < size && temp_column >= 0 && temp_row < size && temp_row >= 0 && arr[row + 1][column + 1][round] != ' ' && arr[row + 1][column + 1][round] != turn && arr[temp_row][temp_column][round] != ' ') {
        if (arr[temp_row][temp_column][round] == turn) {
            is_changable = true;
            break;
        }
        temp_row++;
        temp_column++;
    }
    if (is_changable) {
        temp_row = row + 1;
        for (int i = column + 1; i < temp_column; i++) {
            arr[temp_row][i][round] = turn;
            temp_row++;
        }
    }
    is_changable = false;
    temp_row = row - 2;
    temp_column = column - 2;
    while (temp_column < size && temp_column >= 0 && temp_row < size && temp_row >= 0 && arr[row - 1][column - 1][round] != ' ' && arr[row - 1][column - 1][round] != turn && arr[temp_row][temp_column][round] != ' ') {
        if (arr[temp_row][temp_column][round] == turn) {
            is_changable = true;
            break;
        }
        temp_row--;
        temp_column--;
    }
    if (is_changable) {
        temp_row = row - 1;
        for (int i = column - 1; i > temp_column; i--) {
            arr[temp_row][i][round] = turn;
            temp_row--;
        }
    }
    is_changable = false;
    temp_row = row - 2;
    temp_column = column + 2;
    while (temp_column < size && temp_column >= 0 && temp_row < size && temp_row >= 0 && arr[row - 1][column + 1][round] != ' ' && arr[row - 1][column + 1][round] != turn && arr[temp_row][temp_column][round] != ' ') {
        if (arr[temp_row][temp_column][round] == turn) {
            is_changable = true;
            break;
        }
        temp_row--;
        temp_column++;
    }
    if (is_changable) {
        temp_row = row - 1;
        for (int i = column + 1; i < temp_column; i++) {
            arr[temp_row][i][round] = turn;
            temp_row--;
        }
    }
    is_changable = false;
    temp_row = row + 2;
    temp_column = column - 2;
    while (temp_column < size && temp_column >= 0 && temp_row < size && temp_row >= 0 && arr[row + 1][column - 1][round] != ' ' && arr[row + 1][column - 1][round] != turn && arr[temp_row][temp_column][round] != ' ') {
        if (arr[temp_row][temp_column][round] == turn) {
            is_changable = true;
            break;
        }
        temp_row++;
        temp_column--;
    }
    if (is_changable) {
        temp_row = row + 1;
        for (int i = column - 1; i > temp_column; i--) {
            arr[temp_row][i][round] = turn;
            temp_row++;
        }
    }
}

void ai_nums(char ***arr, int size,int &row, int &column, int round, char turn){
    int max = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int sum = 0;
            int temp_sum = 1;
            if (arr[i][j][round] == ' ') {
                int temp_row = i + 2;
                int temp_column = j;
                while (temp_row < size && temp_row >= 0 && arr[i + 1][j][round] != ' ' &&
                       arr[i + 1][j][round] != turn && arr[temp_row][j][round] != ' ') {
                    temp_sum ++;
                    if (arr[temp_row][j][round] == turn) {
                        sum += temp_sum;
                        break;
                    }
                    temp_row++;
                }
                temp_sum = 1;
                temp_row = i - 2;
                while (temp_row < size && temp_row >= 0 && arr[i - 1][j][round] != ' ' &&
                       arr[i - 1][j][round] != turn && arr[temp_row][j][round] != ' ') {
                    temp_sum ++;
                    if (arr[temp_row][j][round] == turn) {
                        sum += temp_sum;
                        break;
                    }
                    temp_row--;
                }
                temp_sum = 1;
                temp_column = j + 2;
                while (temp_column < size && temp_column >= 0 && arr[i][j + 1][round] != ' ' &&
                       arr[i][j + 1][round] != turn && arr[i][temp_column][round] != ' ') {
                    temp_sum ++;
                    if (arr[i][temp_column][round] == turn) {
                        sum += temp_sum;
                        break;
                    }
                    temp_column++;
                }
                temp_sum = 1;
                temp_column = j - 2;
                while (temp_column < size && temp_column >= 0 && arr[i][j - 1][round] != ' ' &&
                       arr[i][j - 1][round] != turn && arr[i][temp_column][round] != ' ') {
                    temp_sum ++;
                    if (arr[i][temp_column][round] == turn) {
                        sum += temp_sum;
                        break;
                    }
                    temp_column--;
                }
                temp_sum = 1;
                temp_row = i + 2;
                temp_column = j + 2;
                while (temp_column < size && temp_column >= 0 && temp_row < size && temp_row >= 0 &&
                       arr[i + 1][j + 1][round] != ' ' && arr[i + 1][j + 1][round] != turn &&
                       arr[temp_row][temp_column][round] != ' ') {
                    temp_sum ++;
                    if (arr[temp_row][temp_column][round] == turn) {
                        sum += temp_sum;
                        break;
                    }
                    temp_row++;
                    temp_column++;
                }
                temp_sum = 1;
                temp_row = i - 2;
                temp_column = j - 2;
                while (temp_column < size && temp_column >= 0 && temp_row < size && temp_row >= 0 &&
                       arr[i - 1][j - 1][round] != ' ' && arr[i - 1][j - 1][round] != turn &&
                       arr[temp_row][temp_column][round] != ' ') {
                    temp_sum ++;
                    if (arr[temp_row][temp_column][round] == turn) {
                        sum += temp_sum;
                        break;
                    }
                    temp_row--;
                    temp_column--;
                }
                temp_sum = 1;
                temp_row = i - 2;
                temp_column = j + 2;
                while (temp_column < size && temp_column >= 0 && temp_row < size && temp_row >= 0 &&
                       arr[i - 1][j + 1][round] != ' ' && arr[i - 1][j + 1][round] != turn &&
                       arr[temp_row][temp_column][round] != ' ') {
                    temp_sum ++;
                    if (arr[temp_row][temp_column][round] == turn) {
                        sum += temp_sum;
                        break;
                    }
                    temp_row--;
                    temp_column++;
                }
                temp_sum = 1;
                temp_row = i + 2;
                temp_column = j - 2;
                while (temp_column < size && temp_column >= 0 && temp_row < size && temp_row >= 0 &&
                       arr[i + 1][j - 1][round] != ' ' && arr[i + 1][j - 1][round] != turn &&
                       arr[temp_row][temp_column][round] != ' ') {
                    temp_sum ++;
                    if (arr[temp_row][temp_column][round] == turn) {
                        sum += temp_sum;
                        break;
                    }
                    temp_row++;
                    temp_column--;
                }
                if (sum > max){
                    max = sum;
                    row = i;
                    column = j;
                }
            }
        }
    }
}

int menu(){
    int menu_num;
    RenderWindow window(VideoMode(1388, 900), "Welcome to othello");
    Texture texture_main_window;
    bool b = texture_main_window.loadFromFile("C:/Users/1381m/OneDrive/Desktop/Codes/othello_graphic/Resources/menu.png");// full directory address
    Sprite sprite_main_window(texture_main_window);
    sprite_main_window.setScale(0.97, 0.97);
    sprite_main_window.setPosition(0, 0);
     // run the program as long as the window is open
     while (window.isOpen())
     {
         // check all the window's events that were triggered since the last iteration of the loop
         Event event;
         while (window.pollEvent(event))
         {
             if (event.type == Event::KeyPressed)
             {
                 switch (event.key.code){
                     case sf::Keyboard::Num1:
                     case sf::Keyboard::Numpad1:
                         menu_num =1;
                         window.close();
                         break;
                     case sf::Keyboard::Num2:
                     case sf::Keyboard::Numpad2:
                         menu_num = 2;
                         window.close();
                         break ;
                     case sf::Keyboard::Num3:
                     case sf::Keyboard::Numpad3:
                         menu_num = 3;
                         window.close();
                         break;
                 }
             }
             if (event.type == sf::Event::Closed)
                 window.close();
         }
         window.clear(Color::Cyan);
         window.draw(sprite_main_window);
         window.display();
     }
     return menu_num;
 }

int choose_game(){
    int work;
    RenderWindow window(VideoMode(1388, 900), "Welcome to othello");
    Texture texture_main_window;
    bool b = texture_main_window.loadFromFile("C:/Users/1381m/OneDrive/Desktop/Codes/othello_graphic/Resources/choose.png");// full directory address
    Sprite sprite_main_window(texture_main_window);
    sprite_main_window.setScale(1.66, 1.66);
    sprite_main_window.setPosition(0, 0);
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyPressed)
            {
                switch (event.key.code){
                    case sf::Keyboard::Num1:
                    case sf::Keyboard::Numpad1:
                        work =1;
                        window.close();
                        break;
                    case sf::Keyboard::Num2:
                    case sf::Keyboard::Numpad2:
                        work = 2;
                        window.close();
                        break ;
                    case sf::Keyboard::Num3:
                    case sf::Keyboard::Numpad3:
                        work = 3;
                        window.close();
                        break;
                }
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(Color::Black);
        window.draw(sprite_main_window);
        window.display();
    }
    return work;
}

int size (){
    int size;
    RenderWindow window(VideoMode(1388, 900), "Welcome to othello");
    Texture texture_main_window;
    bool b = texture_main_window.loadFromFile("C:/Users/1381m/OneDrive/Desktop/Codes/othello_graphic/Resources/size.png");// full directory address
    Sprite sprite_main_window(texture_main_window);
    sprite_main_window.setScale(1, 1);
    sprite_main_window.setPosition(0, 0);
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyPressed)
            {
                switch (event.key.code){
                    case Keyboard::Num2:
                    case Keyboard::Numpad2:
                        size = 2;
                        window.close();
                        break ;
                    case Keyboard::Num3:
                    case Keyboard::Numpad3:
                        size = 3;
                        window.close();
                        break;
                    case Keyboard::Num4:
                    case Keyboard::Numpad4:
                        size = 4;
                        window.close();
                        break;
                    case Keyboard::Num5:
                    case Keyboard::Numpad5:
                        size = 5;
                        window.close();
                        break;
                    case Keyboard::Num6:
                    case Keyboard::Numpad6:
                        size = 6;
                        window.close();
                        break;
                }
            }
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(Color::Cyan);
        window.draw(sprite_main_window);
        window.display();
    }
    return size * 2;
}

void Score(int n, char ***arr,int &x, int &o, int turn, int round){
    int x_num = 0, o_num = 0;
    for (int j = 0; j < n; j++) {
        for (int k = 0; k < n; k++) {
            if (arr[j][k][round] == 'X') {
                x_num++;
            } else if (arr[j][k][round] == 'O') {
                o_num++;
            }
        }
    }
    x = x_num;
    o = o_num;
}

void Rankings(){
    string a;
    string b;
    ifstream read("Rankings.txt");
    while (getline (read, b)) {
        a.append(b);
    }

    read.close();
    int n = 0;

    for (int i = 0; i < a.size(); i++){
        if (i == a.size() - 1){
            break;
        }
        if (a[i] == ' '){
            n++;
            continue;
        }
        n += 1;
    }
    string *arr = new string[n];
    n = 0;
    string temp = "";
    for (int i = 0; i < a.size(); i++){
        if (i == a.size() - 1){
            temp.append(1, a[i]);
            arr[n] = temp;
            break;
        }
        if (a[i] == ' '){
            arr[n] = temp;
            n++;
            temp = "";
            continue;
        }
        temp.append(1, a[i]);
        n += 1;
    }
    counter(arr, n);
}

void counter(string *arr, int n){
    int *c = new int[n];
    for (int i = 0; i < n; i++){
        c[i] = 1;
    }

    for (int i = 0; i < n; i++){
        if (i != n - 1) {
            for (int j = i + 1; j < n; j++) {
                if (arr[i] == " ") {
                    c[i] = 0;
                    continue;
                } else if (arr[i] == arr[j]) {
                    arr[j] = " ";
                    c[i]++;
                }
            }
        }
        else{
            if (arr[i] == " "){
                c[i] = 0;
            }
            else{
                c[i] = 1;
            }
        }
    }
    string res = "";
    for (int i = 0; i < n; ++i) {
        if (arr[i] == " "){
            continue;
        }
        else{
            string temp = "";
            temp += c[i];
            res =res + arr[i] + " " + temp + "\n";
        }
    }
    RenderWindow window(VideoMode(1388, 900), "Welcome to othello");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(Color::Black);
        Text name1_text;
        name1_text.setFont(font);
        name1_text.setCharacterSize(50);
        name1_text.setColor(Color::Red);
        name1_text.setString(res);
        name1_text.setPosition(0, 0);
        window.draw(name1_text);
        window.display();
    }
}

void ranking_add(string str){
    ofstream write("Rankings.txt", ios::app);
    write << " " << str << endl;
    write.close();
}

void define_server(int n){
     //INITIALIZE Win Socket
     WSADATA Windows_Socket_info;
     WORD VerionOfWindows_Socket = MAKEWORD(2, 2);
     int res = WSAStartup(VerionOfWindows_Socket, &Windows_Socket_info);
     if (res != 0)
     {
         std::cerr << "INITIALIZE WinSock Error : " << res;
         return ;
     }

     //CREATE LISTENING SOCKET
     SOCKET Server_sock = socket(AF_INET, SOCK_STREAM, 0);
     if (Server_sock == SOCKET_ERROR) {
         std::cerr << "ABORT1\n";
         closesocket(Server_sock); //clean up
         WSACleanup(); //clean up
         return ;
     }
     //BIND THE IP ADDRESS AND PORT TO A SOCKET
     sockaddr_in server_address;
     server_address.sin_family = AF_INET;
     server_address.sin_port = htons(PORT);
     server_address.sin_addr.S_un.S_addr = INADDR_ANY; // or inet_addr("127.0.0.1");

     int status = ::bind(Server_sock, (sockaddr*)&server_address, sizeof(server_address));
     if (status == SOCKET_ERROR) {
         std::cerr << "ABORT2\n";
         closesocket(Server_sock); //clean up
         WSACleanup(); //clean up
         return ;
     }
     // TELL WINDSOCK THE SOCKET IS FOE LISTENNING
     status = listen(Server_sock, SOMAXCONN);
     if (status < 0) {
         std::cerr << "ABORT3\n";
         closesocket(Server_sock); //clean up
         WSACleanup(); //clean up
         return ;
     }
     //WAIT FOR CONNECTION
     sockaddr_in client_address;
     int client_address_size = sizeof(client_address);
      NewClient = accept(Server_sock, (sockaddr*)&client_address, &client_address_size);
     if (NewClient == INVALID_SOCKET)
     {
         std::cerr << "ABORT4\n";
         closesocket(NewClient); //clean up
         WSACleanup(); //clean up
     }
     char Host[NI_MAXHOST];
     char Service[NI_MAXSERV];
     ZeroMemory(Host , NI_MAXHOST); // memset(clientip , 0 , NI_MAXHOST);
     ZeroMemory(Service, NI_MAXSERV);
 if (getnameinfo((sockaddr*)&client_address, sizeof(client_address), Host, NI_MAXHOST, Service, NI_MAXSERV , 0) == 0 )
     {
         cout << Host << "Connected on port " << Service;
     }
     else
     {
         inet_ntop(AF_INET, &client_address.sin_addr, Host, NI_MAXHOST);
         cout << Host << "Connected on port " << ntohs(client_address.sin_port);
     }
     //RECEIVE MESSAGE
     char buffer[3];
     buffer[0]=n;
     buffer[1] = -1;
     string message = buffer;
     send(NewClient, message.c_str() , message.length()+1 , 0);

 }

void send(int x, int y){
     char x1 = x;
     char y1 = y;
     char message[BUFFERLEN] = {x1, y1};
     if (send(NewClient, message , 3, 0) < 0)
     {
         cerr << ("\nSend failed");
         exit(1);
     }
 }

int receive(int &x, int &y){
     char buffer[BUFFERLEN];
    int t= recv(NewClient, buffer, BUFFERLEN, 0);

    if(t>0){
     int x1 = buffer[0];
     int y1 = buffer[1];
     x = x1 ;
     y = y1 ;
    cout<<" hhhh "<<t<< " "<<x1<<" "<<y1;
    }
     return t;
 }

