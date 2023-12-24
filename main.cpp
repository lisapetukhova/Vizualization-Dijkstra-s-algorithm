#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <queue>
#include <limits>
using namespace sf;
using namespace std;
#define radius 20

Color whiteColor = Color(255, 255, 255, 200);
Color greenColor = Color(60, 143, 140, 255);

// Вершины графа.
class Vertexx {
public:
    CircleShape shape;
    Text text; // Добавление вершин с центр круга.
    int number; // Порядковый номер вершины.
    bool selected; // Флаг, указывающий на то, выбрана ли вершина.
    float pointX, pointY;
    Color lastColor;
    
    Vertexx(float x, float y, int number) : number(number) {
        shape.setRadius(radius);
        shape.setPosition(x, y);
        pointX = x + radius;
        pointY= y + radius;

        shape.setFillColor( whiteColor);
        shape.setOutlineThickness(4);
        shape.setOutlineColor(sf::Color(104, 12, 133, 255));

        lastColor = whiteColor;
    }

};

// Рёбра графа.
class Edge {
public:
    Vertexx* start;
    Vertexx* end;
    
    //Vertexx* midPoint;
    Edge(Vertexx* start, Vertexx* end) {
        this->start = start;
        this->end = end;
    }
};

class Graph {
public:

    vector<Vertexx*> vertices;
    vector<Edge*> edges;
    Vertexx* previous;
    vector<vector<int>> arrVertex;


    Graph() : vertexCount(0), previous(nullptr) {
    
    }

    ///////////////////////////////////////////////////////////////////
    Vertexx* selectedVertex;

    void selectVertex(Vertexx* vertex) {
        if (selectedVertex != nullptr) {
            selectedVertex->selected = false;
        }
        selectedVertex = vertex;
        selectedVertex->selected = true;
    }

    void changeColor(Color color) {
        if (selectedVertex != nullptr) {
            selectedVertex->shape.setFillColor(color);
            selectedVertex->lastColor=color;
        }
    }
    Color getColor() {
        if (selectedVertex != nullptr) {
            return selectedVertex->lastColor;
        }
    }
    void connectVertices(Vertexx* vertex) {
        if (selectedVertex != nullptr && vertex != nullptr) {
            edges.push_back(new Edge(selectedVertex, vertex));
            Color tempColor = selectedVertex->shape.getFillColor();
            selectedVertex->shape.setFillColor(vertex->shape.getFillColor());
            vertex->shape.setFillColor(tempColor);
            selectedVertex = nullptr;
        }
    }
    ///////////////////////////////////////////////////////////////////

    void addVertex(float x, float y ) {

        Vertexx* newVertex = new Vertexx(x, y, vertexCount + 1);
        vertexCount++;
        vertices.push_back(newVertex);

        previous = newVertex;
    }

    void removeVertex() {
        vertexCount = 0;
    }

    void addEdge(Vertexx* start, Vertexx* end) {

            edges.push_back(new Edge(start, end));

    }

    void drawEdges(RenderWindow& window) {
        for (Edge* edge : edges) {
            Vertex start = edge->start->shape.getPosition();
            Vertex end = edge->end->shape.getPosition();
            Vertex line[] = { start, end };
            window.draw(line, 5, Lines);
        }
    }

private:
    int vertexCount; // Счётчик вершин для их нумерации.

};

int main() {

    bool selected5Menu = false; // Выбрано ли "Создание графа". Для выводв 
    bool selectedStartAlgorithm = false;
    bool selectedEndAlgorithm = false;
    bool deletedGraph = false; // Был ли удалён граф.
    bool pathVertex = false;
    int enteredNumber = NULL;
    string enteredNumberString = "";
    int enteredStart = NULL;
    string enteredStartString = "";
    int enteredEnd = NULL;
    string enteredEndString = "";
    bool enteredNumberBool = false;
    bool graphCreated = false;
    int matrX, matrY;// координаты в матрице смежности
    // Создаём окно.
    RenderWindow window(VideoMode(1280, 920), "Dijkstra's Algorithm");

    // Создаём вектор кругов
    vector<CircleShape> circles;

    // Создаем окно, разделённое на две части.
    RectangleShape leftRect(Vector2f(850, 720));
    RectangleShape rightRect(Vector2f(850, 720));
    RectangleShape lowRect(Vector2f(window.getSize().x, window.getSize().y - 200));


    // Цвет и позиция для левого прямоугольника.
    leftRect.setFillColor(Color(136, 89, 150, 255));
    leftRect.setPosition(0, 0);

    // Цвет и позиция для правого прямоугольника.
    rightRect.setFillColor(Color(60, 143, 140, 255));
    rightRect.setPosition(850, 0);

    // Цвет и позиуия для нижнего прямоугольника.
    lowRect.setFillColor(Color(121, 148, 142, 255));
    lowRect.setPosition(0, 720);

    // Разделяющая линия.
    Vertex line[] = {
      Vertex(Vector2f(850, 0)),
      Vertex(Vector2f(850, 720))
    };

    Vertex lineHorizontally[] = {
      Vertex(Vector2f(0, 720)),
      Vertex(Vector2f(1280, 720))
    };

    // Создаем тень для разделяющей линии.
    //RectangleShape lineShadow(Vector2f(5, 720));
    //lineShadow.setFillColor(Color(0, 0, 0, 50)); // Цвет тени - чёрный с прозрачностью 100.
    //lineShadow.setPosition(850, 0); // Позиция тени - слева от разделяющей линии.

    // Устанавливаем картинку.
    Image icon;
    if (!icon.loadFromFile("Image/icon8.png")) {
        cout << "Упс, картинка потерялась.";
        return 1;
    }
    window.setIcon(48, 48, icon.getPixelsPtr());

    // Установка шрифта.
    Font font;
    if (!font.loadFromFile("ofont.ru_Boozy.ttf")) {
        cout << "Упс, шрифт затерялся. Поищите его.";
        return 1;
    }

    Font font1;
    if (!font1.loadFromFile("Vogue Bold.ttf")) {
        cout << "Упс, шрифт затерялся. Поищите его.";
        return 1;
    }

    Text weightEdge("", font, 40);
    weightEdge.setFillColor(Color::White);

    vector<Text> previousTexts;

    Text StartEnd("", font, 40);
    StartEnd.setFillColor(Color::White);

    // Создание текста.
    Text menu1(L"1) Создать ребро.", font, 40);
    menu1.setPosition(860, 10);

    Text menu2(L"2) Удалить граф.", font, 40);
    menu2.setPosition(860, 60);

    Text menu3(L"3) Начать алгоритм. ", font, 40);
    menu3.setPosition(860, 110);

    Text menu4(L" Выход. ", font, 40);
    menu4.setPosition(1150, 670);

    Text menu5(L"Введите вес ребра _ ", font, 40);
    menu5.setPosition(860, 160);

    Text textHelp1(L"1) Нажмите на левую кнопку мыши, чтобы создать вершину. ", font, 30);
    textHelp1.setPosition(10, 730);
    Text textHelp2(L"2) Нажмите на правую кнопку мыши, чтобы выделить вершину.", font, 30);
    textHelp2.setPosition(10, 770);
    Text textHelp3(L"3) Если выполнен п.2, то нажмите на 'C', чтобы выделить вершину цветом. ", font, 30);
    textHelp3.setPosition(10, 810);
    Text textHelp4(L"4) Ребро создаётся тогда, когда выделено цветом две вершины. ", font, 30);
    textHelp4.setPosition(10, 850);
    Text textHelp5(L"5) Чтобы зафиксировать вес ребра, нажвите 'Enter'. ", font, 30);
    textHelp5.setPosition(10, 888);

    Text startVertex(L"Начальная вершина: ", font, 40);
    startVertex.setPosition(860, 210);
    Text endVertex(L"Конечная вершина: ", font, 40);
    endVertex.setPosition(860, 260);

    Text startNum("", font, 40);
    startNum.setFillColor(sf::Color::White);
    startNum.setPosition(1000, 210);

    Text endNum("", font, 40);
    endNum.setFillColor(Color::White);
    endNum.setPosition(950, 260);

    // Создание графа.
    Graph graph;

    int CountVertex = 0; // Счётчик вершин.
    int coloredVertex = 0;

    window.clear(Color::Black);

    // Отрисовка кадров.
    while (window.isOpen()) {

        Event event;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Right) {
                    for (Vertexx* vertex : graph.vertices) {
                        if (vertex->shape.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                            graph.selectVertex(vertex);
                            break;
                        }
                    }
                }
            }

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::C) {
                    if (graph.getColor() == whiteColor) {
                        if (coloredVertex < 2) {
                            graph.changeColor(greenColor);
                            coloredVertex++;
                        }
                    }
                    else  if (graph.getColor() == greenColor) {

                        graph.changeColor(whiteColor);
                        coloredVertex--;
                    }


                }

                if (selectedStartAlgorithm == false) {
                    if (event.key.code == Keyboard::Enter)
                    {

                        if (enteredNumberBool) {
                            enteredNumber = stoi(enteredNumberString);
                            enteredNumberString = " ";
                            enteredNumberBool = false;

                            graph.arrVertex[matrX][matrY] = enteredNumber;
                            graph.arrVertex[matrY][matrX] = enteredNumber;

                            weightEdge.setPosition(1200, 160);

                            Text previousText;
                            previousText.setFont(font);
                            previousText.setCharacterSize(40);
                            previousText.setFillColor(Color::White);
                            previousText.setString(to_string(enteredNumber));
                            for (Edge* edge : graph.edges) {

                                int FirstPoint = edge->start->pointX + edge->end->pointX;
                                int SecondPoint = edge->start->pointY + edge->end->pointY;
                                previousText.setPosition(FirstPoint / 2, SecondPoint / 2);
                                //weightEdge.setPosition(FirstPoint / 2, SecondPoint / 2);

                            }
                            previousTexts.push_back(previousText);
                        }
                    }
                }
            }

            if (event.type == Event::TextEntered)
            {
                if (event.text.unicode < 128)
                {
                    char inputChar = static_cast<char>(event.text.unicode);
                    if (inputChar >= '0' && inputChar <= '9')
                    {
                        enteredNumberString += inputChar;
                        weightEdge.setString(enteredNumberString);
                    }
                }
            }//////////////////////////////////////////////////////////////////////////////////////////

            // Обработка курсора.
            if (event.type == Event::MouseMoved) {
                Vector2i mousePos = Mouse::getPosition(window);

                if (menu1.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    menu1.setFillColor(Color(27, 82, 63, 255));
                }
                else {
                    menu1.setFillColor(Color::White);
                }

                if (menu2.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    menu2.setFillColor(Color(27, 82, 63, 255));
                }
                else {
                    menu2.setFillColor(Color::White);
                }

                if (menu3.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    menu3.setFillColor(Color(27, 82, 63, 255));
                }
                else {
                    menu3.setFillColor(Color::White);
                }

                if (menu4.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    menu4.setFillColor(Color(27, 82, 63, 255));
                }
                else {
                    menu4.setFillColor(Color::White);
                }

            }

            // Обработка нажатий мыши.
            if (event.type == Event::MouseButtonPressed) {

                if (event.mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);
                    if (!graphCreated) {

                        // Обработка курсора в левой части экрана.
                        if ((mousePos.x + radius * 2 < 850) and (mousePos.y + radius * 2 < 720)) {

                            bool canAddVertex = true;
                            for (Vertexx* vertex : graph.vertices) {
                                float distance = sqrt(pow(mousePos.x - vertex->shape.getPosition().x, 2) +
                                    pow(mousePos.y - vertex->shape.getPosition().y, 2));

                                if (distance < vertex->shape.getRadius() * 4) {
                                    canAddVertex = false;
                                    break;
                                }
                            }

                            if (canAddVertex) {

                                graph.addVertex(mousePos.x, mousePos.y);
                                CountVertex++;

                            }
                        }
                    }
                    // Обработка курсора в правой части экрана.
                    if (menu1.getGlobalBounds().contains(mousePos.x, mousePos.y) and CountVertex > 1) {
                        if (coloredVertex == 2) {
                            selected5Menu = true;
                            deletedGraph = false;
                            Vertexx* start = NULL;
                            Vertexx* finish = NULL;

                            for (int i = 0; i < CountVertex; i++)
                            {
                                if (graph.vertices[i]->lastColor == greenColor) {
                                    if (start == NULL)
                                    {
                                        start = graph.vertices[i];
                                        matrX = i;
                                    }
                                    else {
                                        finish = graph.vertices[i];
                                        matrY = i;
                                    }
                                    graph.vertices[i]->shape.setFillColor(whiteColor);
                                    graph.vertices[i]->lastColor = whiteColor;
                                    coloredVertex--;
                                }
                            }
                            graph.edges.push_back(new Edge(start, finish));
                            enteredNumberBool = true;
                            graphCreated = true;
                            if (graph.arrVertex.empty()) {
                                for (int i = 0; i < CountVertex; i++)
                                {

                                    graph.arrVertex.push_back(vector<int>());
                                    for (int j = 0; j < CountVertex; j++)
                                    {

                                        graph.arrVertex[i].push_back(0);

                                    }
                                }

                                //int s = 0;
                            }
                            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        }
                    }
                    else if (menu2.getGlobalBounds().contains(mousePos.x, mousePos.y)) {

                        graph.vertices.clear();
                        graph.edges.clear();
                        graph.removeVertex();
                        previousTexts.clear();
                        CountVertex = 0;
                        coloredVertex = 0;
                        deletedGraph = true;
                        graphCreated = false;

                    }
                    else if (menu3.getGlobalBounds().contains(mousePos.x, mousePos.y)) {

                        // CountVertex - количество вершин.
                        selectedStartAlgorithm = true;

                        if (event.type == Event::TextEntered) 
                        {
                            if (event.text.unicode < 128) 
                            {
                                char inputChar = static_cast<char>(event.text.unicode);
                                if (inputChar >= '0' && inputChar <= '9')
                                {
                                    enteredStartString += inputChar;
                                    startNum.setString(enteredStartString);
                                }
                            }
                                                        
                        }

                        if (event.type == Event::KeyPressed)
                        {
                            if (event.key.code == Keyboard::Enter)
                            {
                                enteredStart = stoi(enteredStartString);
                                enteredStartString = " ";
                            }
                        }

                        int FirstVertex = enteredStart - 1;

                        if (enteredStart != 0) {
                            selectedEndAlgorithm = true;
                        }

                        if (selectedEndAlgorithm) {

                            if (event.type == Event::TextEntered)
                            {
                                if (event.text.unicode < 128)
                                {
                                    char inputChar = static_cast<char>(event.text.unicode);
                                    if (inputChar >= '0' && inputChar <= '9')
                                    {
                                        enteredEndString += inputChar;
                                        startNum.setString(enteredEndString);
                                    }
                                }

                            }

                            if (event.type == Event::KeyPressed)
                            {
                                if (event.key.code == Keyboard::Enter)
                                {
                                    enteredEnd = stoi(enteredEndString);
                                    enteredEndString = " ";
                                }
                            }
                        }

                        int LastVertex = enteredEnd - 1;

                        int Infinity = numeric_limits<int>::max(); // Т.к. расстояние до других вершин неизвестно, приравниваем к бесконечности.

                        // Наш заданный граф с помощью матрицы смежностей:
                        //graph.arrVertex[matrX][matrY];
                        //graph.arrVertex[matrY][matrX];

                        if (matrX == matrY) {
                            graph.arrVertex[matrX][matrY] = 0;
                            graph.arrVertex[matrY][matrX] = 0;
                        }

                        if (matrX != matrY and (graph.arrVertex[matrX][matrY] < 1) and (graph.arrVertex[matrY][matrX] < 1)) {
                            graph.arrVertex[matrX][matrY] = Infinity;
                            graph.arrVertex[matrY][matrX] = Infinity;
                        }

                        // Массив с весом между вершинами:
                        vector <int> lenght(CountVertex, Infinity);

                        // Очередь с приоритетом для вершин:
                        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> QueueVertex;

                        // Начало алгоритма. Начинаем с первой вершины.
                        lenght[FirstVertex] = 0; // Начало (исходящий узел графа) равен 0 (т.к. это расстояние вершины до самой себя).
                        QueueVertex.push({ 0, FirstVertex }); // Вставляем в приоритетную очередь начальную вершину.


                        // Массив для отслежования посещённых вершин
                        vector<bool> visited(CountVertex, false);
                        visited[FirstVertex] = true; // Помечаем начальную вершину как посещённую.

                        // Массив для хранения предыдущей вершины.
                        vector<int> previous(CountVertex, -1);

                        // Шаги повторяем до тех пор, пока на графе есть непосещенные точки.
                        while (!QueueVertex.empty()) {
                            int distance = -QueueVertex.top().first;
                            int vertex = QueueVertex.top().second; // Следующая вершина.
                            QueueVertex.pop();

                            // Если рассматриваемая длина больше той, что уже хранится, то идём дальше.
                            if (distance > lenght[vertex]) {
                                continue;
                            }

                            // Перебираем все вершины.
                            for (int v = 0; v < CountVertex; v++) {
                                int dist = graph.arrVertex[vertex][v];
                                if (dist != Infinity) {
                                    if (lenght[v] > lenght[vertex] + dist) {
                                        lenght[v] = lenght[vertex] + dist;
                                        QueueVertex.push({ -lenght[v], v });
                                        visited[v] = true; // Помечаем вершину как посещённую.
                                        previous[v] = vertex; // Запоминаем предыдущую вершину.
                                    }

                                }
                            }
                        }

                        // В случае несвязанных вершин, вес пути до вершины приравниваем к бесконечности.
                        if (lenght[LastVertex] == Infinity) {

                            pathVertex = true;
                            Text textINF("INFINITY", font, 50);
                            textINF.setPosition(860, 320);
                            /* В случае несвязанных вершин выводится строчка "INFINITY", 
                            чтобы это отследить надо ввести флажок. */
                        }
                        else if (lenght[LastVertex != Infinity]) {  // Вывод кратчайшего пути:
                            pathVertex = true;
                            // Вывод кратчайшего пути:
                            if (previous[LastVertex] != -1 || FirstVertex == LastVertex) {
                                // Если вершина посещена и мы дошли до конца, то:
                                vector<int> path; // Вектор, для хранения вершин кратчайшего пути.
                                for (int vertex = LastVertex; vertex != -1; vertex = previous[vertex]) {
                                    path.push_back(vertex);
                                }
                                // Переворачиваем, так как рассматривался путь от конечной до исходной вершины.
                                reverse(path.begin(), path.end());

                                // Выводим кратчайший путь:
                                string str = "\nКратчайший путь: [";
                                for (int i = 0; i < path.size(); i++) {
                                    str += to_string(path[i] + 1);
                                    if (i != path.size() - 1) {
                                        str += "] -> [";
                                    }
                                }
                                str += "]: " + to_string(lenght[LastVertex]);

                                Text text(str, font, 50);
                                text.setFillColor(Color::Black);
                                text.setPosition(10, 700); // Позиция текста смещается на 30 пикселей вниз от предыдущего текста
                                //window.draw(text);
                            }
                        }
                        else { // Иначе по каким-то причинам мы не дошли до конца. Сообщение об отсутсвии пути.
                            Text text("\nПути не существует", font, 20);
                            text.setPosition(10, 10 + (CountVertex * 30)); // Позиция текста смещается на 30 пикселей вниз от предыдущего текста
                            //window.draw(text);
                        }
                      

                    }
                               
                        else if (menu4.getGlobalBounds().contains(mousePos.x, mousePos.y)) {

                            return 1;

                        }
                    }
                }

            }

            // Отрисовка прямоугольников.
            window.draw(leftRect);
            window.draw(rightRect);
            window.draw(lowRect);
            window.draw(line, 20, Lines);
            window.draw(lineHorizontally, 20, Lines);

            // Рисуем тень.
            //window.draw(lineShadow);

            // Рисуем меню.
            window.draw(menu1);
            window.draw(menu2);
            window.draw(menu3);
            window.draw(menu4);

            window.draw(textHelp1);
            window.draw(textHelp2);
            window.draw(textHelp3);
            window.draw(textHelp4);
            window.draw(textHelp5);


            for (auto& previousText : previousTexts) {
                window.draw(previousText);
            }

            if (selected5Menu) {

                window.draw(menu5);

            }

            if (selectedStartAlgorithm) {

                window.draw(startVertex);
                window.draw(startNum);
                if (selectedEndAlgorithm) {
                    window.draw(endVertex);
                }

            }

            for (Edge* edge : graph.edges) {

                Vertex line[] = {
                        Vertex(Vector2f(edge->start->pointX, edge->start->pointY)),
                        Vertex(Vector2f(edge->end->pointX , edge->end->pointY))
                };
                window.draw(line, 5, Lines);
            }

            // Отрисовка графа.
            for (Vertexx* vertex : graph.vertices) {
                window.draw(vertex->shape); // Отрисовка круга.
                // Создание текста.
                Text text(to_string(vertex->number), font1, 35);
                text.setFillColor(Color(148, 33, 158, 255));

                // Вычисление координат центра круга.
                float centerX = vertex->shape.getPosition().x + vertex->shape.getRadius();
                float centerY = vertex->shape.getPosition().y + vertex->shape.getRadius();

                // Установка позиции текста в центре круга.
                text.setPosition(centerX - 10, centerY - 25);

                // Отрисовка текста.
                window.draw(text);
            }

            if (deletedGraph) {

                // Отрисовка прямоугольников.
                window.draw(leftRect);
                window.draw(rightRect);
                window.draw(line, 10, Lines);
                window.draw(lineHorizontally, 20, Lines);

                // Рисуем тень.
                //window.draw(lineShadow);

                // Рисуем меню.
                window.draw(menu1);
                window.draw(menu2);
                window.draw(menu3);
                window.draw(menu4);

                window.draw(textHelp1);
                window.draw(textHelp2);
                window.draw(textHelp3);
                window.draw(textHelp4);
                window.draw(textHelp5);

                for (auto& previousText : previousTexts) {
                    window.draw(previousText);
                }

                // Отрисовка графа.
                for (Vertexx* vertex : graph.vertices) {
                    window.draw(vertex->shape); // Отрисовка круга.
                    // Создание текста.
                    Text text(to_string(vertex->number), font1, 35);
                    text.setFillColor(Color(148, 33, 158, 255));

                    // Вычисление координат центра круга.
                    float centerX = vertex->shape.getPosition().x + vertex->shape.getRadius();
                    float centerY = vertex->shape.getPosition().y + vertex->shape.getRadius();

                    // Установка позиции текста в центре круга.
                    text.setPosition(centerX - 10, centerY - 25);

                    // Отрисовка текста.
                    window.draw(text);
                }

                // Отрисовка линий.
                for (Edge* edge : graph.edges) {
                    Vertex line[] = {
                        Vertex(Vector2f(edge->start->shape.getPosition().x, edge->start->shape.getPosition().y)),
                        Vertex(Vector2f(edge->end->shape.getPosition().x, edge->end->shape.getPosition().y))
                    };
                    window.draw(line, 5, Lines);
                }

            }

            window.display();

        }

        return 0;
    }
