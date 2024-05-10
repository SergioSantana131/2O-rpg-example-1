
#include "Player.h"
#include <iostream>
#include "../Utils.h"
#include <cstring>
#include "Combat.h"
using namespace std;
using namespace combat_utils;


bool compareSpeed(Enemy *a, Enemy *b) {
    return a->getSpeed() > b->getSpeed();
}

//costructor
Player::Player(char name[30], int health, int attack, int defense, int speed, char arm[20], int experience, int level) : Character(name, health, attack, defense, speed, true, arm, experience, level) {

}

void Player::doAttack(Character *target) {
    int rolledAttack = getRolledAttack(getAttack());
    int trueDamage = target->getDefense() > rolledAttack ? 0 : rolledAttack - target->getDefense();
    target->takeDamage(trueDamage);
}

void Player::takeDamage(int damage) {
    setHealth(health - damage);
    cout << "HAS MATADO " << damage << " Dano con un "<< getArm() << endl;
    if (health <= 0) {
        cout << "Has muerto JUEGO TERMINADO " << endl;
    }
}


void Player::flee(vector<Enemy*> enemies) {
    std::sort(enemies.begin(), enemies.end(), compareSpeed);
    Enemy *fastestEnemy = enemies[0];
    bool fleed = false;
    if (this->getSpeed() > fastestEnemy->getSpeed()) {
        fleed = true;
    } else {
        srand(time(NULL));
        int chance = rand() % 100;
        cout << "chence de huir " << chance << endl;
        if (fleed = chance > 90) {
            fleed = true;
        } else {
            cout << "No te puedes ir volando, lUCHA!! " << endl;
        }
    }

    this->fleed = fleed;

}

void Player::emote() {
    cout<<"Se burla de ti" << endl;
}


Character* Player::getTarget(vector<Enemy *> enemies) {
        cout << "Elige un objetivo" << endl;
    int targetIndex = 0;
    for(int i = 0; i < enemies.size(); i++) {
        cout << i << ". " << enemies[i]->getName() << endl;
    }
    cin >> targetIndex;

    return enemies[targetIndex];
}

void Player::gainExperience(Enemy* enemy) {
    // Saber si el enemigo murio para tomar su experiencia
    if (enemy && enemy->health <= 0) {
        // Sumar la experiencia obtenida por derrotar al enemigo
        experience += enemy->experience;
        // Llamar a la función LevelUp para manejar el nivel y la experiencia restante
        LevelUp();
    }
}

void Player::LevelUp() {
    // Saber si el jugador tiene 100 o mas de experiencia para aumentar el Level
    while (experience >= 100) {
        level++;
        // Reiniciar
        experience -= 100;

        // Atributos aumentados
        int healthGain = 10;
        int attackGain = 5;
        int defenseGain = 5;
        health += healthGain;
        attack += attackGain;
        defense += defenseGain;
        // Puntos ganados TOTAL
        int totalPointsGained = healthGain + attackGain + defenseGain;
    }
}


Action Player::takeAction(vector<Enemy*>enemies) {
    int option = 0;
    cout<<"////////////////"<<endl;
    cout<<"Elige una accion"<<endl;
    cout<<"1. Ataque"<<endl;
    cout<<"2. Volar"<<endl;

    cin >> option;
    Character* target = nullptr;


    Action myAction;

    myAction.speed = this->getSpeed();
    myAction.subscriber = this;

    switch (option) {
        case 1:
            target = getTarget(enemies);
            myAction.target = target;
            //1.
            myAction.action = [this, target]() {
                doAttack(target);
            };
            break;
        case 2:
            myAction.action = [this, enemies]() {
                flee(enemies);
            };
            break;
        default:
            cout << "Opción inválida" << endl;
            break;
    }


    return myAction;
}