#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <string>
using namespace sf;
using namespace std;
#define radius 20

// ������� �����.
class Vertexx {
public:
    CircleShape shape;
    Text text; // ���������� ������ � ����� �����.
    int number; // ���������� ����� �������.
    bool selected; // ����, ����������� �� ��, ������� �� �������.
    
    Vertexx(float x, float y, int number) : number(number) {
        shape.setRadius(radius);
        shape.setPosition(x, y);
        shape.setFillColor(Color(255, 255, 255, 200));
        shape.setOutlineThickness(3);
        shape.setOutlineColor(sf::Color(104, 12, 133, 255));
    }

};

// и��� �����.
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

    Graph() : vertexCount(0), previous(nullptr) {}

    vector<Vertexx*> vertices;
    vector<Edge*> edges;
    Vertexx* previous;

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
            window.draw(line, 2, Lines);
        }
    }

private:
    int vertexCount; // ������� ������ ��� �� ���������.

};

int main() {

    bool selected4Menu = false; // ������� �� "�������� �����". ��� ������ 
    bool deletedGraph = false; // ��� �� ����� ����.

    // ������ ����.
    RenderWindow window(VideoMode(1280, 720), "Dijkstra's Algorithm");

    // ������ ������ ������
    vector<CircleShape> circles;

    // ������� ����, ���������� �� ��� �����.
    RectangleShape leftRect(Vector2f(850, 720));
    RectangleShape rightRect(Vector2f(850, 720));

    // ���� � ������� ��� ������ ��������������.
    leftRect.setFillColor(Color(136, 89, 150, 255));
    leftRect.setPosition(0, 0);

    // ���� � ������� ��� ������� ��������������.
    rightRect.setFillColor(Color(60, 143, 140, 255));
    rightRect.setPosition(850, 0);

    // ����������� �����.
    Vertex line[] = {
      Vertex(Vector2f(850, 0)),
      Vertex(Vector2f(850, 720))
    };

    // ������� ���� ��� ����������� �����.
    RectangleShape lineShadow(Vector2f(5, 720));
    lineShadow.setFillColor(Color(0, 0, 0, 50)); // ���� ���� - ������ � ������������� 100.
    lineShadow.setPosition(850, 0); // ������� ���� - ����� �� ����������� �����.

    // ������������� ��������.
    Image icon;
    if (!icon.loadFromFile("Image/icon8.png")) {
       cout << "���, �������� ����������.";
      return 1;
    }
    window.setIcon(48, 48, icon.getPixelsPtr());

    // ��������� ������.
    Font font;
    if (!font.loadFromFile("ofont.ru_Boozy.ttf")) {
        cout << "���, ����� ���������. ������� ���.";
        return 1;
    }

    Font font1;
    if (!font1.loadFromFile("Vogue Bold.ttf")) {
        cout << "���, ����� ���������. ������� ���.";
        return 1;
    }

    // �������� ������.
    Text menu1(L"1) ������� �����.", font, 40);
    menu1.setPosition(860, 10);

    Text menu2(L"2) ������� ����.", font, 40);
    menu2.setPosition(860, 60);

    Text menu3(L"3) �����.", font, 40);
    menu3.setPosition(860, 110);

    Text menu4(L"������� ��� ����: ", font, 40);
    menu4.setPosition(860, 160);

    // �������� �����.
    Graph graph;

    int CountVertex = 0; // ������� ������.
    int coloredVertex = 0;

    window.clear(Color::Black);

    // ��������� ������.
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
                    graph.changeColor(Color(60, 143, 140, 255));
                    coloredVertex++;
                }
                if (event.key.code == Keyboard::S) {
                    if (coloredVertex < 3) {
                        for (Vertexx* vertex : graph.vertices) {
                            if (vertex->shape.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y)) {
                                graph.connectVertices(vertex);
                                break;
                            }
                        }
                    }
                }
            }

            // ��������� �������.
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

               /* if (menu4.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    menu4.setFillColor(Color(27, 82, 63, 255));
                }
                else {
                    menu4.setFillColor(Color::White);
                }*/
            }

            // ��������� ������� ����.
            if (event.type == Event::MouseButtonPressed) {

                //int countSelected = 0;// ���������� ���������� ������       
                //// �������� ����� ����� �������.
                //if (event.mouseButton.button == Mouse::Right) {
                //    Vector2i mousePos = Mouse::getPosition(window);
                //    Vertexx* selectedVertex = nullptr;
                //    Vertexx* firstSelectedVertex = nullptr;
                //    for (Vertexx* vertex : graph.vertices) {
                //        if (vertex->shape.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                //            if (vertex->selected) {
                //                if (firstSelectedVertex == nullptr) {
                //                    firstSelectedVertex = vertex;
                //                }
                //                else {
                //                    graph.addEdge(firstSelectedVertex, vertex);
                //                    firstSelectedVertex->selected = false;
                //                    vertex->selected = false;
                //                    firstSelectedVertex = nullptr;
                //                }
                //            }
                //            else {
                //                vertex->selected = true;
                //                vertex->shape.setFillColor(Color(100, 19, 107, 200));
                //            }
                //            break;
                //        }
                //    }
                //}

                if (event.mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);
                    
                    // ��������� ������� � ����� ����� ������.
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
                    
                    // ��������� ������� � ������ ����� ������.
                    if (menu1.getGlobalBounds().contains(mousePos.x, mousePos.y) and CountVertex > 1) {

                        selected4Menu = true;
                        deletedGraph = false;
                        //if ()

                        
                    }
                    else if (menu2.getGlobalBounds().contains(mousePos.x, mousePos.y)) {

                        graph.vertices.clear();
                        graph.edges.clear();  
                        graph.removeVertex();
                        CountVertex = 0;
                        deletedGraph = true;

                    }
                    else if (menu3.getGlobalBounds().contains(mousePos.x, mousePos.y)) {

                        return 1;

                    }
                }
            }

        }

        // ��������� ���������������.
        window.draw(leftRect);
        window.draw(rightRect);
        window.draw(line, 10, Lines);

        // ������ ����.
        window.draw(lineShadow);

        // ������ ����.
        window.draw(menu1);
        window.draw(menu2);
        window.draw(menu3);

        if (selected4Menu) {

            window.draw(menu4);

        }
        
        for (Edge* edge : graph.edges) {

            Vertex line[] = {
                    Vertex(Vector2f(edge->start->shape.getPosition().x + radius/2, edge->start->shape.getPosition().y + radius / 2)),
                    Vertex(Vector2f(edge->end->shape.getPosition().x + radius / 2, edge->end->shape.getPosition().y + radius / 2))
                };
            window.draw(line, 2, Lines);
        }

        // ��������� �����.
        for (Vertexx* vertex : graph.vertices) {
            window.draw(vertex->shape); // ��������� �����.
            // �������� ������.
            Text text(to_string(vertex->number), font1, 35);
            text.setFillColor(Color(148, 33, 158, 255));

            // ���������� ��������� ������ �����.
            float centerX = vertex->shape.getPosition().x + vertex->shape.getRadius();
            float centerY = vertex->shape.getPosition().y + vertex->shape.getRadius();

            // ��������� ������� ������ � ������ �����.
            text.setPosition(centerX-10, centerY-25);

            // ��������� ������.
            window.draw(text);
        }        

        if (deletedGraph) {

            // ��������� ���������������.
            window.draw(leftRect);
            window.draw(rightRect);
            window.draw(line, 10, Lines);

            // ������ ����.
            window.draw(lineShadow);

            // ������ ����.
            window.draw(menu1);
            window.draw(menu2);
            window.draw(menu3);

            // ��������� �����.
            for (Vertexx* vertex : graph.vertices) {
                window.draw(vertex->shape); // ��������� �����.
                // �������� ������.
                Text text(to_string(vertex->number), font1, 35);
                text.setFillColor(Color(148, 33, 158, 255));

                // ���������� ��������� ������ �����.
                float centerX = vertex->shape.getPosition().x + vertex->shape.getRadius();
                float centerY = vertex->shape.getPosition().y + vertex->shape.getRadius();

                // ��������� ������� ������ � ������ �����.
                text.setPosition(centerX - 10, centerY - 25);

                // ��������� ������.
                window.draw(text);
            }

            // ��������� �����.
            for (Edge* edge : graph.edges) {
                Vertex line[] = {
                    Vertex(Vector2f(edge->start->shape.getPosition().x, edge->start->shape.getPosition().y)),
                    Vertex(Vector2f(edge->end->shape.getPosition().x, edge->end->shape.getPosition().y))
                };
                window.draw(line, 2, Lines);
            }

        }

        window.display();

    }

    return 0;
}