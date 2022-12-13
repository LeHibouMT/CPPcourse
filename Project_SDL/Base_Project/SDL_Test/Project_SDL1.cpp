// SDL_Test.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "Project_SDL1.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <numeric>
#include <random>
#include <string>
#include <list>
#include <vector>
#include<iterator>
#include <math.h>

void init() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0)
    throw std::runtime_error("init():" + std::string(SDL_GetError()));

  // Initialize PNG loading
  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags))
    throw std::runtime_error("init(): SDL_image could not initialize! "
                             "SDL_image Error: " + std::string(IMG_GetError()));
  // un timer sera nécessaire
}

namespace {
// Defining a namespace without a name -> Anonymous workspace
// Its purpose is to indicate to the compiler that everything
// inside of it is UNIQUELY used within this source file.

SDL_Surface* load_surface_for(const std::string& path,
                              SDL_Surface* window_surface_ptr) {

  // Helper function to load a png for a specific surface
  // See SDL_ConvertSurface
}
} // namespace



// animal //////////////////////////////////////////////////////////////////////////////////////////////////////////////
animal::animal(const animal& pa) { 
    this->window_surface_ptr_ = pa.window_surface_ptr_;
    this->image_ptr_ = pa.image_ptr_;
    this->x = pa.x;
    this->y = pa.y;
    this->l = pa.l;
    this->w = pa.w;
    this->ms = 0;
    this->dead = false;
}

animal::animal(const std::string& file_path, SDL_Surface* window_surface_ptr, int pX, int pY, int pL, int pW, int pMS)
{
    this->image_ptr_ = IMG_Load(file_path.c_str());
    this->window_surface_ptr_ = window_surface_ptr;
    this->x = pX;
    this->y = pY;
    this->l = pL;
    this->w = pW;
    this->ms = pMS;
    this->dead = false;
}

animal::~animal() {
    SDL_FreeSurface(this->image_ptr_);
    SDL_FreeSurface(this->window_surface_ptr_);
}

void animal::draw() {
    SDL_BlitScaled(this->image_ptr_, this->rect_, this->window_surface_ptr_, NULL);
}

int animal::getpositionX() {
    return this->x;
}

int animal::getpositionY() {
    return this->y;
}

int animal::getms() {
    return this->ms;
}

int animal::getdirection() {
    return this->direction;
}

void animal::setposition(int pX, int pY) {
    this->x = pX;
    this->y = pY;
}

void animal::changedirection(int d) {
    this->direction = d;
}

void animal::changems(int ms) {
    this->ms = ms;
}

bool animal::detectanimal(animal* a) {
    return (((this->x <= a->x && this->x + this->l >= a->x) || // contact in x
        (this->x >= a->x && this->x <= a->x + a->l)) &&
        ((this->y <= a->y && this->y + this->w >= a->y) || // contact in y
            (this->y >= a->y && this->y <= a->y + a->w)));
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



// sheep //////////////////////////////////////////////////////////////////////////////////////////////////////////////
sheep::sheep(SDL_Surface* window_surface_ptr) : animal("sheep.png", window_surface_ptr, 0, 0, 1, 1, 3) {
}

void sheep::move() {
    // the sheep will do X number of step at a speed equal to ms then will change direction randomly, same for step
    if (this->step == 0) {
        this->step = rand() % 8;
        this->changedirection((rand() % 4));
        this->changems(3);
    }
    else {
        if (this->getdirection() == 0) this->setposition(this->getpositionX(), this->getpositionY() - this->getms());
        if (this->getdirection() == 1) this->setposition(this->getpositionX() + this->getms(), this->getpositionY());
        if (this->getdirection() == 2) this->setposition(this->getpositionX(), this->getpositionY() + this->getms());
        if (this->getdirection() == 3) this->setposition(this->getpositionX() - this->getms(), this->getpositionY());
        this->step--;
    }
}

time_t sheep::gettimer() {
    return this->timer;
}

int sheep::getismale() {
    return this->ismale;
}

void sheep::changetimer(time_t t) {
    this->timer = t;
}

void sheep::run(std::vector<animal*> v) {
    // find the direction of the nearest sheep
    if (v.empty());
    else {
        double min = 7; // the sheep will change direction if near <7 blocks from a wolf
        int dir = 0;
        std::vector<animal*>::iterator it;
        for (it = v.begin(); it != v.end(); it++) {
            if (sqrt(pow(this->getpositionX() - (*it)->getpositionX(), 2) + pow((this->getpositionY() - (*it)->getpositionY()), 2)) < min) {
                min = sqrt(pow(this->getpositionX() - (*it)->getpositionX(), 2) + pow((this->getpositionY() - (*it)->getpositionY()), 2));
                if (pow(this->getpositionX() - (*it)->getpositionX(), 2) < pow((this->getpositionY() - (*it)->getpositionY()), 2)) { // it means that the wolf should move in x
                    if (this->getpositionX() - (*it)->getpositionX() > 0) {
                        this->changedirection(1); 
                        this->changems(5);
                        this->step = 1;
                    } // it means that the wolf is at the right side of the sheep
                    else { 
                        this->changedirection(3); 
                        this->changems(5);
                        this->step = 1;
                    }
                }
                else {
                    if (this->getpositionY() - (*it)->getpositionY() > 0) {
                        this->changedirection(2);
                        this->changems(5);
                        this->step = 1;
                    }
                    else {
                        this->changedirection(0);
                        this->changems(5);
                        this->step = 1;
                    }
                }
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



// wolf //////////////////////////////////////////////////////////////////////////////////////////////////////////////
wolf::wolf(SDL_Surface* window_surface_ptr) : animal("wolf.png", window_surface_ptr, 50, 50, 1, 1, 4) {
    this->changedirection(-1); // so the wolf doesn't move if there is no sheep
}

void wolf::findsheep(std::vector<animal*> v) {
    // find the direction of the nearest sheep
    if (v.empty()) this->changedirection(-1);
    else {
        double min = 10000.0; // we take a huge number to be sure
        std::vector<animal*>::iterator it;
        for (it = v.begin(); it != v.end(); it++) {
            if (sqrt(pow(this->getpositionX() - (*it)->getpositionX(), 2) + pow((this->getpositionY() - (*it)->getpositionY()), 2)) < min) {
                min = sqrt(pow(this->getpositionX() - (*it)->getpositionX(), 2) + pow((this->getpositionY() - (*it)->getpositionY()), 2));
                if (pow(this->getpositionX() - (*it)->getpositionX(), 2) < pow((this->getpositionY() - (*it)->getpositionY()), 2)) { // it means that the wolf should move in x
                    if (this->getpositionX() - (*it)->getpositionX() > 0) this->changedirection(3); // it means that the wolf is at the right side of the sheep
                    else this->changedirection(1);
                }
                else {
                    if (this->getpositionY() - (*it)->getpositionY() > 0) this->changedirection(0);
                    else this->changedirection(2);
                }
            }
        }
    }
}

void wolf::finddog(std::vector<animal*> v) {
    // find the direction of the nearest dog
    if (v.empty());
    else {
        double min = 6.0; // we take 
        std::vector<animal*>::iterator it;
        for (it = v.begin(); it != v.end(); it++) {
            if (sqrt(pow(this->getpositionX() - (*it)->getpositionX(), 2) + pow((this->getpositionY() - (*it)->getpositionY()), 2)) < min) {
                min = sqrt(pow(this->getpositionX() - (*it)->getpositionX(), 2) + pow((this->getpositionY() - (*it)->getpositionY()), 2));
                if (pow(this->getpositionX() - (*it)->getpositionX(), 2) < pow((this->getpositionY() - (*it)->getpositionY()), 2)) { // it means that the wolf should move in x
                    if (this->getpositionX() - (*it)->getpositionX() > 0) this->changedirection(1); // it means that the wolf is at the right side of the dog
                    else this->changedirection(3);
                }
                else {
                    if (this->getpositionY() - (*it)->getpositionY() > 0) this->changedirection(2);
                    else this->changedirection(0);
                }
            }
        }
    }
}

void wolf::move() {
    // the wolf will move in the direction of the nearest sheep
    if (this->getdirection() == 0) this->setposition(this->getpositionX(), this->getpositionY() - this->getms());
    if (this->getdirection() == 1) this->setposition(this->getpositionX() + this->getms(), this->getpositionY());
    if (this->getdirection() == 2) this->setposition(this->getpositionX(), this->getpositionY() + this->getms());
    if (this->getdirection() == 3) this->setposition(this->getpositionX() - this->getms(), this->getpositionY());
}

time_t wolf::gettimer() {
    return this->timer;
}

void wolf::changetimer(time_t t) {
    this->timer = t;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



// shepherd //////////////////////////////////////////////////////////////////////////////////////////////////////////////
shepherd::shepherd(SDL_Surface* window_surface_ptr, SDL_Event event) : animal("sheep.png", window_surface_ptr, 0, 0, 2, 2, 4) {
    this->event = event;
}

void shepherd::move() {
    while (SDL_PollEvent(&this->event)) {
        switch (event.type) {
            /* Look for a keypress */
        case SDL_KEYDOWN:
            /* Check the SDLKey values and move change the coords */
            switch (event.key.keysym.sym) {
            case SDLK_z:
                this->setposition(this->getpositionX(), this->getpositionY() - this->getms());
                break;
            case SDLK_d:
                this->setposition(this->getpositionX() + this->getms(), this->getpositionY());
                break;
            case SDLK_s:
                this->setposition(this->getpositionX(), this->getpositionY() + this->getms());
                break;
            case SDLK_q:
                this->setposition(this->getpositionX() - this->getms(), this->getpositionY());
                break;
            default:
                break;
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



// dog //////////////////////////////////////////////////////////////////////////////////////////////////////////////
dog::dog(SDL_Surface* window_surface_ptr, SDL_Event event) : animal("wolf.png", window_surface_ptr, 0, 0, 2, 2, 6) {
    this->event = event;
}

int dog::getrealrelative(shepherd s) {
    if (this->getpositionX() - s.getpositionX() > 0) {
        if (this->getpositionY() - s.getpositionY() > 0) return 1;
        else return 0;
    }
    else {
        if (this->getpositionY() - s.getpositionY() > 0) return 2;
        else return 3;
    }
}

void dog::circleshepard(shepherd s) {
    // we first ensure that the dog is not far from the shepherd, if not the dog runs straight to the shepherd until he is close enough
    if (sqrt(pow(this->getpositionX() - s.getpositionX(), 2) + pow((this->getpositionY() - s.getpositionY()), 2)) < 6) {
        if (pow(this->getpositionX() - s.getpositionX(), 2) < pow((this->getpositionY() - s.getpositionY()), 2)) { // it means that the wolf should move in x
            if (this->getpositionX() - s.getpositionX() > 0) this->changedirection(3); // it means that the wolf is at the right side of the sheep
            else this->changedirection(1);
        }
        else {
            if (this->getpositionY() - s.getpositionY() > 0) this->changedirection(0);
            else this->changedirection(2);
        }
    }
    // then we make him circle around the shepherd, the idea is to make him go to the next zone represented by dog.relative
    else {
        if (this->getrealrelative(s) == this->relative) this->relative = (this->relative + 1) % 4; // we need to change dog.relative
        if (this->relative == 0) this->changedirection(2); // then the dog change direction to go to the next zone
        if (this->relative == 1) this->changedirection(3);
        if (this->relative == 2) this->changedirection(0);
        if (this->relative == 3) this->changedirection(1);
    }
}

void dog::movenocommand() {
    if (this->getdirection() == 0) this->setposition(this->getpositionX(), this->getpositionY() - this->getms());
    if (this->getdirection() == 1) this->setposition(this->getpositionX() + this->getms(), this->getpositionY());
    if (this->getdirection() == 2) this->setposition(this->getpositionX(), this->getpositionY() + this->getms());
    if (this->getdirection() == 3) this->setposition(this->getpositionX() - this->getms(), this->getpositionY());
}

void dog::move() {
    if (this->iscommand) this->movecommand();
    else this->movenocommand();
}
void dog::changecommand(bool b) {
    this->iscommand = b;
}

bool dog::getiscommand() {
    return this->iscommand;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



// ground //////////////////////////////////////////////////////////////////////////////////////////////////////////////
ground::ground(SDL_Surface* window_surface_ptr) : window_surface_ptr_{ window_surface_ptr } {
    if (window_surface_ptr_ == NULL)
    {
        throw std::runtime_error("Error : " + std::string(SDL_GetError()));
    }
}

ground::~ground() {
    SDL_FreeSurface(window_surface_ptr_);
}

void ground::add_animal(sheep* s) {
    this->vsheep.push_back(s);
    this->vanimalsheep.push_back(s);
}

void ground::add_animal(wolf* w) {
    this->vwolf.push_back(w);
    this->vanimalwolf.push_back(w);
}

void ground::remove_animal(sheep* s) {
    std::vector<sheep*>::iterator it = this->vsheep.begin();
    while (it != this->vsheep.end())
    {
        if ((*it) == s)
        {
            it = this->vsheep.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void ground::remove_animal(wolf* w) {
    std::vector<wolf*>::iterator it = this->vwolf.begin();
    while (it != this->vwolf.end())
    {
        if ((*it) == w)
        {
            it = this->vwolf.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void ground::starvewolf() {
    for (wolf* w : this->vwolf) {
        if (w->gettimer() > 10) this->remove_animal(w);
    }
}

void ground::eatsheep() {
    for (wolf* w : this->vwolf) {
        for (sheep* s : this->vsheep) {
            if (w->detectanimal(s)) {
                this->remove_animal(s);
                w->changetimer(0);
            }
        }
    }
}

void ground::reproducesheep() {
    for (sheep* s : this->vsheep) {
        for (sheep* ss : this->vsheep) {
            if (s->detectanimal(ss)) {
                if ((s->getismale() == 0 && s->gettimer() > 7 && ss->getismale() == 1)) {
                    sheep* a = &sheep::sheep(this->window_surface_ptr_);
                    this->add_animal(a);
                    s->changetimer(0);
                }
                if ((ss->getismale() == 0 && ss->gettimer() > 7 && s->getismale() == 1)) {
                    sheep* a = &sheep::sheep(this->window_surface_ptr_);
                    this->add_animal(a);
                    ss->changetimer(0);
                }
            } 
        }
    }
}

void ground::update() {
    this->starvewolf();
    this->eatsheep();
    this->reproducesheep();
    for (sheep* s : this->vsheep) {
        s->draw();
        s->run(this->vanimalwolf); // we determine if the sheep need to change direction or not
        s->move();
        s->changetimer(clock() - s->gettimer());
    }
    for (wolf* w : this->vwolf) {
        w->draw();
        w->findsheep(this->vanimalsheep); // we determine if the wolf need to change direction or not
        w->finddog(this->vanimaldog); // we determine if the wolf should stay away from a dog instead of going to a sheep
        w->move();
        w->changetimer(clock() - w->gettimer());
    }
    for (dog* d : this->vdog) {
        d->draw();
        d->move();
    }
}

int ground::getframerate() {
    return this->framerate;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



// application //////////////////////////////////////////////////////////////////////////////////////////////////////////////
application::application(unsigned n_sheep, unsigned n_wolf) {
    this->window_ptr_ = SDL_CreateWindow("Project_SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, frame_width, frame_height, 0);
    if (window_ptr_ == NULL)
    {
        throw std::runtime_error("Error : " + std::string(SDL_GetError()));
    }

    this->window_surface_ptr_ = SDL_GetWindowSurface(window_ptr_);
    if (window_surface_ptr_ == NULL) {
        throw std::runtime_error("Error : " + std::string(SDL_GetError()));
    }

    this->ground = ground::ground(this->window_surface_ptr_);

    // we add the sheep
    for (int i = 1; i < n_sheep; i++) {
        auto* sheep = &sheep::sheep(window_surface_ptr_);
        this->ground.add_animal(sheep);
    }

    // we add the wolf
    for (int i = 1; i < n_wolf; i++) {
        auto* wolf = &sheep::sheep(window_surface_ptr_);
        this->ground.add_animal(wolf);
    }

    this->ground.update();
}

application::~application() {
    SDL_FreeSurface(window_surface_ptr_);
    SDL_DestroyWindow(window_ptr_);
}

int application::loop(unsigned period) {
    time_t start = SDL_GetTicks();
    while (start < 100) {
        this->ground.update();
        SDL_Delay(1/this->ground.getframerate()); // to have the right framerate
    }
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////