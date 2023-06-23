// HanoiTowersSFML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

const float PILLAR_YPOS = 400.f;
const float PILLAR_BASE_WIDTH = 200.f;
const float PILLAR_BASE_HEIGHT = 40.f;
const float PILLAR_SHAFT_WIDTH = 40.f;
const float PILLAR_SHAFT_HEIGHT = 200.f;

const float PILLAR_START_XPOS = 120.f;
const float PILLAR_TRANSITION_XPOS = 360.f;
const float PILLAR_FINISH_XPOS = 600.f;

const float PLATE_RADIUS_MIN = 50.f;        // arbitrary value
const float PLATE_RADIUS_MAX = 190.f;       // base width - 10.f
const float PLATE_HEIGHT_MIN = 10.f;        // arbitrary value
const float PLATE_HEIGHT_MAX = 40.f;        // arbitrary value

const unsigned int WINDOW_WIDTH = 900;
const unsigned int WINDOW_HEIGHT = 600;


class Pillar {
public:
    Pillar(const float xpos) : 
        base{ sf::Vector2f(PILLAR_BASE_WIDTH, PILLAR_BASE_HEIGHT) }, shaft{ sf::Vector2f(PILLAR_SHAFT_WIDTH, PILLAR_SHAFT_HEIGHT) }, window{ nullptr }
    {
        base.setOrigin(0.f, 0.f); shaft.setOrigin(0.f, 0.f);
        base.setPosition(xpos, PILLAR_YPOS);
        // Set the shaft on top of the base
        shaft.setPosition(xpos + 0.5f*(PILLAR_BASE_WIDTH - PILLAR_SHAFT_WIDTH), PILLAR_YPOS - PILLAR_SHAFT_HEIGHT);

        // Brown Color: 964B00
        base.setFillColor(sf::Color(0x96, 0x4B, 0x00)); 
        shaft.setFillColor(sf::Color(0x96, 0x4B, 0x00));

        // Set default top plate position
        topPlatePosition = { xpos + 0.5f*PILLAR_BASE_WIDTH, PILLAR_YPOS - PILLAR_BASE_HEIGHT };
    }
    void setWindow(sf::RenderWindow* window) noexcept {
        this->window = window;
    }
    void draw() const {
        window->draw(base);
        window->draw(shaft);
    }
    sf::Vector2f getTopPlatePosition() const {
        return topPlatePosition;
    }
    void setTopPlatePosition(const float plateYPosition) {
        topPlatePosition.y = plateYPosition;
    }
    void updateTopPlatePosition(const float previousPlateHeight) {
        topPlatePosition.y -= previousPlateHeight;
    }
private:
    sf::RectangleShape base;
    sf::RectangleShape shaft;
    sf::RenderWindow* window;
    sf::Vector2f topPlatePosition;
};

class Plate : public sf::RectangleShape {
public:
    Pillar* currentPillar = nullptr;
};

// Recursive function
const int animation_step_count = 1000;
std::vector<Plate*>& solveHanoiTowers(std::vector<Plate*>& plates, Pillar& start, Pillar& transition, Pillar& finish, 
    sf::RenderWindow* window, std::vector<Plate*> leftover_plates = {}) {

    const unsigned int n = plates.size();

    // Stop condition
    if (n == 0) {
        return plates; // aka empty vector
    };

    std::vector<Plate*> plates_new(n - 1); // Contains all of plates except the first element
    // Fill plates_new with all elements from plates (except the last one, aka the largest plate)
    for (unsigned int i = 0; i < plates.size() -1; i++) {
        plates_new[i] = plates[i];
    }

    // Remember the plates that we haven't copied to plates_new, so that SFML will keep drawing them
    leftover_plates.emplace_back(plates[n - 1]);

    // 1st recursive call
    std::vector<Plate*> returnPlate = solveHanoiTowers(plates_new, start, finish, transition, window, leftover_plates);
    // update current plates (their position) with returnPlate
    for (unsigned int i = 0; i < returnPlate.size(); i++) {
        plates[i] = returnPlate[i];
    }

    // Move last plate to finish
    const sf::Vector2f plate_size = plates[n - 1]->getSize();
    sf::Vector2f finish_coord = finish.getTopPlatePosition();
    finish_coord.x -= plate_size.x / 2.f; // finish_coord is the target position
    const sf::Vector2f move_vec = finish_coord - plates[n - 1]->getPosition(); // Get the vector that will get us from plates[n-1] to the next pillar
    const float plate_height = plate_size.y;

    // We remove a plate from the current pillar (so the next one we draw has to sit lower)
    plates[n - 1]->currentPillar->updateTopPlatePosition(-(plate_height + 5.f));

    const sf::Vector2f move_vec_step = move_vec / static_cast<float>(animation_step_count); // Get a fraction of move_vec for the animation
    // Begin animation
    for (int i = 0; i < animation_step_count; i++) {

        sf::Event evt;
        while (window->pollEvent(evt)) {
            if (evt.type == sf::Event::Closed) {
                window->close();
            }
        }
        window->clear();

        // DRAW HERE
        start.draw();
        transition.draw();
        finish.draw();

        for (const Plate* p : plates) window->draw(*p);
        for (const Plate* lp : leftover_plates) window->draw(*lp);

        window->display();

        plates[n - 1]->move(move_vec_step);
    }
    finish.updateTopPlatePosition(plate_height + 5.f); // We add a plate to the finish pillar (so the next one we draw has to sit higher)
    plates[n - 1]->currentPillar = &finish; // Actually moving the plate

    // 2nd recursice call
    returnPlate = solveHanoiTowers(plates_new, transition, start, finish, window, leftover_plates); // returnPlate contains n-1 elements
    // update current plates (their position) with returnPlate
    for (unsigned int i = 0; i < returnPlate.size(); i++) {
        plates[i] = returnPlate[i];
    }

    // Return current state of plates
    return plates;
}

int main()
{
    int n = 3;
    std::string buffer;
    std::cout << "Set number of plates (default 3): ";
    std::getline(std::cin, buffer);
    if (!buffer.empty()) {
        n = atoi(buffer.c_str());
    }
    std::cout << "Hanoi Towers with n = " << n << std::endl;

    // Create our window
    sf::RenderWindow window{ sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Hanoi Towers" };

    // Pillars
    Pillar start{ PILLAR_START_XPOS }, transition{ PILLAR_TRANSITION_XPOS }, finish{ PILLAR_FINISH_XPOS };
    start.setWindow(&window);
    transition.setWindow(&window);
    finish.setWindow(&window);

    // Plates
    std::vector<Plate> plates(n);
    std::vector<Plate*> plates_ref(n);

    sf::Vector2f plate_size = { PLATE_RADIUS_MAX,  PLATE_HEIGHT_MAX };
    sf::Vector2f plate_size_offset = { (PLATE_RADIUS_MAX - PLATE_RADIUS_MIN) / static_cast<float>(n), (PLATE_HEIGHT_MAX - PLATE_HEIGHT_MIN) / static_cast<float>(n) };

    // 125 = start.x_pos + 5, 360 = 400 - base_height
    sf::Vector2f plate_position = { PILLAR_START_XPOS + 0.5f*(PILLAR_BASE_WIDTH - PLATE_RADIUS_MAX), PILLAR_YPOS - PILLAR_BASE_HEIGHT }; 
    sf::Vector2f plate_position_offset = { plate_size_offset.x / 2.f, -PLATE_HEIGHT_MAX };
    for (int i = n - 1; i >= 0; i--) {
        plates[i].setSize(plate_size);
        plates[i].setOrigin(0.f, 0.f);
        plates[i].setPosition(plate_position);
        plates[i].setFillColor(sf::Color::Red);
        plates[i].currentPillar = &start;
        plate_size -= plate_size_offset;
        plate_position += plate_position_offset;
        plate_position_offset.y += plate_size_offset.y;

        plates_ref[i] = &plates[i];
    }

    start.setTopPlatePosition(plate_position.y - 5.f);

    // Recursion at last
    plates_ref = solveHanoiTowers(plates_ref, start, transition, finish, &window);

    // Main Loop : show elements, which should all be on the last pillar in order
    while (window.isOpen()) {
        sf::Event evt;
        while (window.pollEvent(evt)) {
            if (evt.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();

        // DRAW HERE
        start.draw();
        transition.draw();
        finish.draw();

        for (const Plate& p : plates) window.draw(p);

        window.display();
    }

    return 0;
}
