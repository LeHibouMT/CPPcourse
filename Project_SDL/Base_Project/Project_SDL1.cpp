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

namespace
{
    // Defining a namespace without a name -> Anonymous workspace
    // Its purpose is to indicate to the compiler that everything
    // inside of it is UNIQUELY used within this source file.

    SDL_Surface* load_surface_for(const std::string& path,
        SDL_Surface* window_surface_ptr) {

        // Helper function to load a png for a specific surface
        // See SDL_ConvertSurface
        SDL_Surface* optimizedSurface = NULL;
        //Load image at specified path
        SDL_Surface* loadedSurface = IMG_Load(path.c_str());
        if (loadedSurface == NULL)
            throw std::runtime_error("Unable to load image " + std::string(path.c_str()) + "!SDL_image Error");
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface(loadedSurface, window_surface_ptr->format, NULL);
        if (optimizedSurface == NULL)
            throw std::runtime_error("Unable to optimize image! SDL_image Error");

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);

        return optimizedSurface;
    }
} // namespace



// animal //////////////////////////////////////////////////////////////////////////////////////////////////////////////

animal::animal(const std::string& file_path, SDL_Surface* window_surface_ptr, int pX, int pY, int pL, int pW, int pMS)
{
    this->image_ptr_ = load_surface_for(file_path.c_str(),window_surface_ptr);
    this->window_surface_ptr_ = window_surface_ptr;
    this->x = pX;
    this->y = pY;
    this->l = pL;
    this->w = pW;
    this->ms = pMS;
    this->dead = false;
}

void animal::draw() {
    SDL_Rect vRect = {(int)this->x, (int)this->y, 0, 0};
    SDL_BlitSurface(this->image_ptr_, NULL, this->window_surface_ptr_, &vRect);
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
sheep::sheep(SDL_Surface* window_surface_ptr) : animal("media/sheep.png", window_surface_ptr, (rand() % frame_width - frame_boundary / 2) + frame_boundary / 2,
    (rand() % frame_height - frame_boundary / 2) + frame_boundary / 2, 30, 30, 1) {
}

void sheep::move() {
    // the sheep will do X number of step at a speed equal to ms then will change direction randomly, same for step
    if (this->step == 0) {
        this->step = rand() % 8;
        this->changedirection((rand() % 4));
        this->changems(1);
    }
    if (this->getdirection() == 0 && this->getpositionY() - this->getms() > 0) this->setposition(this->getpositionX(), this->getpositionY() - this->getms());
    else
    {
        if (this->getdirection() == 1 && this->getpositionX() + this->getms() < frame_width - frame_boundary) this->setposition(this->getpositionX() + this->getms(), this->getpositionY());
        else
        {
            if (this->getdirection() == 2 && this->getpositionY() + this->getms() < frame_height - frame_boundary) this->setposition(this->getpositionX(), this->getpositionY() + this->getms());
            else
            {
                if (this->getdirection() == 3 && this->getpositionX() - this->getms() > 0) this->setposition(this->getpositionX() - this->getms(), this->getpositionY());
                else this->changedirection((rand() % 4));
            }
        }
    }
    this->step--;
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

void sheep::setstep(int i)
{
    this->step = i;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



// wolf //////////////////////////////////////////////////////////////////////////////////////////////////////////////
wolf::wolf(SDL_Surface* window_surface_ptr) : animal("media/wolf.png", window_surface_ptr, (rand() % frame_width - frame_boundary / 2) + frame_boundary / 2,
    (rand() % frame_height - frame_boundary / 2) + frame_boundary / 2, 30, 30, 1) {
    this->changedirection(-1); // so the wolf doesn't move if there is no sheep
}

void wolf::findsheep(std::vector<sheep*> v) {
    // find the direction of the nearest sheep
    if (v.empty()) this->changedirection(-1);
    else {
        double min = 10000.0; // we take a huge number to be sure
        std::vector<sheep*>::iterator it;
        for (it = v.begin(); it != v.end(); it++) {
            if (sqrt(pow(this->getpositionX() - (*it)->getpositionX(), 2) + pow((this->getpositionY() - (*it)->getpositionY()), 2)) < min) {
                min = sqrt(pow(this->getpositionX() - (*it)->getpositionX(), 2) + pow((this->getpositionY() - (*it)->getpositionY()), 2));
                if (pow(this->getpositionX() - (*it)->getpositionX(), 2) < pow((this->getpositionY() - (*it)->getpositionY()), 2)) { // it means that the wolf should move in x
                    if (this->getpositionY() - (*it)->getpositionY() > 0) this->changedirection(0);
                    else this->changedirection(2);
                }
                else {
                    if (this->getpositionX() - (*it)->getpositionX() > 0) this->changedirection(3); // it means that the wolf is at the right side of the sheep
                    else this->changedirection(1);
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
    if (this->getdirection() == 0 && this->getpositionY() - this->getms() > 0) this->setposition(this->getpositionX(), this->getpositionY() - this->getms());
    if (this->getdirection() == 1 && this->getpositionX() + this->getms() < frame_width - frame_boundary) this->setposition(this->getpositionX() + this->getms(), this->getpositionY());
    if (this->getdirection() == 2 && this->getpositionY() + this->getms() < frame_height - frame_boundary) this->setposition(this->getpositionX(), this->getpositionY() + this->getms());
    if (this->getdirection() == 3 && this->getpositionX() - this->getms() > 0) this->setposition(this->getpositionX() - this->getms(), this->getpositionY());
}

time_t wolf::gettimer() {
    return this->timer;
}

void wolf::changetimer(time_t t) {
    this->timer = t;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



// shepherd //////////////////////////////////////////////////////////////////////////////////////////////////////////////
shepherd::shepherd(SDL_Surface* window_surface_ptr, SDL_Event event) : animal("media/img.png", window_surface_ptr, (rand() % frame_width - frame_boundary / 2) + frame_boundary / 2,
    (rand() % frame_height - frame_boundary / 2) + frame_boundary / 2, 30, 30, 10) {
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
dog::dog(SDL_Surface* window_surface_ptr) : animal("media/dog.png", window_surface_ptr, (rand() % frame_width - frame_boundary / 2) + frame_boundary / 2,
    (rand() % frame_height - frame_boundary / 2) + frame_boundary / 2, 30, 30, 1) {
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
    if (sqrt(pow(this->getpositionX() - s.getpositionX(), 2) + pow((this->getpositionY() - s.getpositionY()), 2)) > 25) {
        if (pow(this->getpositionX() - s.getpositionX(), 2) < pow((this->getpositionY() - s.getpositionY()), 2)) { // it means that the wolf should move in x
            if (this->getpositionY() - s.getpositionY() > 0) this->changedirection(0);
            else this->changedirection(2);
        }
        else {
            if (this->getpositionX() - s.getpositionX() > 0) this->changedirection(3); // it means that the wolf is at the right side of the sheep
            else this->changedirection(1);
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

void ground::add_animal(dog* dg) {
    this->vdog.push_back(dg);
    this->vanimaldog.push_back(dg);
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
            this->vwolf.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void ground::starvewolf() {
    for (wolf* w : this->vwolf) {
        if (w->gettimer() > 750) this->remove_animal(w);
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
                if ((s->getismale() == 0 && s->gettimer() > 750 && ss->getismale() == 1)) {
                    sheep* a = new sheep(this->window_surface_ptr_);
                    a->setposition(s->getpositionX(), s->getpositionY());
                    a->changetimer(0);
                    this->add_animal(a);
                    s->changetimer(0);
                }
                if ((ss->getismale() == 0 && ss->gettimer() > 750 && s->getismale() == 1)) {
                    sheep* a = new sheep(this->window_surface_ptr_);
                    a->setposition(ss->getpositionX(), ss->getpositionY());
                    a->changetimer(0);
                    this->add_animal(a);
                    ss->changetimer(0);
                }
            } 
        }
    }
}

void ground::update() {
    SDL_FillRect(this->window_surface_ptr_, NULL, 0x000000);
    this->starvewolf();
    this->eatsheep();
    this->reproducesheep();
    for (sheep* s : this->vsheep) {
        auto v = this->vwolf;
        if (v.empty());
        else {
            double min = 30; // the sheep will change direction if near <7 blocks from a wolf
            int dir = 0;
            int newms = 3;
            std::vector<wolf*>::iterator it;
            for (it = v.begin(); it != v.end(); it++) {
                if (sqrt(pow(s->getpositionX() - (*it)->getpositionX(), 2) + pow((s->getpositionY() - (*it)->getpositionY()), 2)) < min) {
                    min = sqrt(pow(s->getpositionX() - (*it)->getpositionX(), 2) + pow((s->getpositionY() - (*it)->getpositionY()), 2));
                    if (pow(s->getpositionX() - (*it)->getpositionX(), 2) < pow((s->getpositionY() - (*it)->getpositionY()), 2)) { // it means that the wolf should move in x
                        if (s->getpositionX() - (*it)->getpositionX() > 0) {
                            s->changedirection(1);
                            s->changems(newms);
                            s->setstep(2);
                        } // it means that the wolf is at the right side of the sheep
                        else {
                            s->changedirection(3);
                            s->changems(newms);
                            s->setstep(2);
                        }
                    }
                    else {
                        if (s->getpositionY() - (*it)->getpositionY() > 0) {
                            s->changedirection(2);
                            s->changems(newms);
                            s->setstep(2);
                        }
                        else {
                            s->changedirection(0);
                            s->changems(newms);
                            s->setstep(2);
                        }
                    }
                }
            }
        }
        s->move();
        s->changetimer(s->gettimer() + 1);
        s->draw();
    }
    for (wolf* w : this->vwolf) {
        w->findsheep(this->vsheep); // we determine if the wolf need to change direction or not
        auto v = this->vdog;
        if (v.empty());
        else {
            double min = 170; // the sheep will change direction if near <7 blocks from a wolf
            int dir = 0;
            int newms = 3;
            std::vector<dog*>::iterator it;
            for (it = v.begin(); it != v.end(); it++) {
                if (sqrt(pow(w->getpositionX() - (*it)->getpositionX(), 2) + pow((w->getpositionY() - (*it)->getpositionY()), 2)) < min) {
                    min = sqrt(pow(w->getpositionX() - (*it)->getpositionX(), 2) + pow((w->getpositionY() - (*it)->getpositionY()), 2));
                    if (pow(w->getpositionX() - (*it)->getpositionX(), 2) < pow((w->getpositionY() - (*it)->getpositionY()), 2)) { // it means that the wolf should move in x
                        if (w->getpositionX() - (*it)->getpositionX() > 0) {
                            w->changedirection(1);
                        } // it means that the wolf is at the right side of the sheep
                        else {
                            w->changedirection(3);
                        }
                    }
                    else {
                        if (w->getpositionY() - (*it)->getpositionY() > 0) {
                            w->changedirection(2);
                        }
                        else {
                            w->changedirection(0);
                        }
                    }
                }
            }
        }
        w->move();
        //w->changetimer(clock() - w->gettimer());
        w->draw();
        w->changetimer(w->gettimer() + 1);
    }
    for (dog* d : this->vdog) {
        d->circleshepard(this->player);
        d->move();
        d->draw();
    }
    this->eatsheep();
    this->player.move();
    this->player.draw();
}

int ground::getframerate() {
    return this->framerate;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



// application //////////////////////////////////////////////////////////////////////////////////////////////////////////////
application::application(unsigned n_sheep, unsigned n_wolf) {
    this->window_ptr_ = SDL_CreateWindow("Project_SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, frame_width, frame_height, 0);
    if (window_ptr_ == NULL)
    {
        throw std::runtime_error("Error : " + std::string(SDL_GetError()));
    }

    SDL_Renderer* rend;
    rend = SDL_CreateRenderer(this->window_ptr_, -1, 0);
    SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
    SDL_RenderClear(rend);


    this->window_surface_ptr_ = SDL_GetWindowSurface(window_ptr_);
    if (window_surface_ptr_ == NULL) {
        throw std::runtime_error("Error : " + std::string(SDL_GetError()));
    }

    this->grd = new ground(this->window_surface_ptr_);
    SDL_BlitSurface(load_surface_for("media/img.png", this->window_surface_ptr_), NULL, this->window_surface_ptr_, NULL);
    SDL_UpdateWindowSurface(this->window_ptr_);

    // we add the sheep
    for (int i = 1; i <= n_sheep; i++) {
        sheep* shp = new sheep(window_surface_ptr_);
        (*this->grd).add_animal(shp);
    }

    // we add the wolf
    for (int i = 1; i <= n_wolf; i++) {
        wolf* wlf = new wolf(window_surface_ptr_);
        (*this->grd).add_animal(wlf);
    }
    for (int i = 1; i <= 1; i++) {
        dog* dg = new dog(window_surface_ptr_);
        (*this->grd).add_animal(dg);
    }
    (*this->grd).update();
}

int application::loop(unsigned period)
{
    auto start = SDL_GetTicks();
    bool quit = false;
    //Tant que ne pas cliqué sur la croix et temps pas écoulé
    while (!quit && (SDL_GetTicks() - start < period * 1000)) {

        this->grd->update();
        SDL_UpdateWindowSurface(this->window_ptr_);

        //Renvoie 1 s'il y a un événement en attente ou 0 s'il n'y en a pas de disponible.
        while (SDL_PollEvent(&this->event)) {
            if (this->event.type == SDL_QUIT) {
                quit = true;
                break;
            }
        }
        SDL_Delay(300 * frame_time);
    }
    return quit;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////