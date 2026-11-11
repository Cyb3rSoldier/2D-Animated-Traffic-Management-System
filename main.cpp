//  2D Animated Traffic Management System  (C++ / OpenGL / FreeGLUT)

#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

// ---------------------------------------------------------------------
//  Constants & Globals
// ---------------------------------------------------------------------
const int   SCR_W = 800, SCR_H = 600;
const float GAP = 30.0f;
const int   MAX_VEHICLES = 35;
const int   MAX_AMBULANCES = 3;

// Stop lines for intersection
const float STOP_R = 310.0f;
const float STOP_L = 490.0f;
const float STOP_U = 210.0f;
const float STOP_D = 390.0f;

enum LightState { RED, GREEN, YELLOW };
enum VType      { CAR, BUS, TRUCK, AMBULANCE };

struct Vehicle {
    float x, y, speed;
    int   dir;        // 0=right, 1=left, 2=up, 3=down
    int   type;
    float r, g, b;
    bool  remove;
};

struct Building {
    float x, y, w, h, d; // d is depth for 3D effect
    float r, g, b;
};

std::vector<Vehicle> vehicles;
std::vector<Building> buildings;

bool  paused   = false;
bool  isNight  = false;
float speedMul = 1.0f;
float dim      = 1.0f;

int selSide = 0;
char msgText[80] = "";
int  msgUntil    = 0;

const float PHASE_DUR[6] = { 5, 2, 2, 5, 2, 2 };
int   phase      = 0;
float phaseTime  = 0.0f;
int   emergAxis  = -1;

// ---------------------------------------------------------------------
//  Utilities
// ---------------------------------------------------------------------
float vehLen(int t) {
    switch (t) {
        case BUS:       return 100.0f;
        case TRUCK:     return 90.0f;
        case AMBULANCE: return 70.0f;
        default:        return 60.0f;
    }
}
float vehW(int t) {
    switch (t) {
        case BUS:       return 32.0f;
        case TRUCK:     return 32.0f;
        case AMBULANCE: return 28.0f;
        default:        return 28.0f;
    }
}
const char* sideName(int s) {
    switch (s) {
        case 0:  return "LEFT";  case 1:  return "RIGHT";
        case 2:  return "BOTTOM";case 3:  return "TOP";
        default: return "RANDOM";
    }
}
float rnd01() { return rand() / (float)RAND_MAX; }
void setMessage(const char* text) {
    snprintf(msgText, sizeof(msgText), "%s", text);
    msgUntil = glutGet(GLUT_ELAPSED_TIME) + 2000;
}

// ---------------------------------------------------------------------
//  Drawing Primitives
// ---------------------------------------------------------------------
void drawRect(float x, float y, float w, float h, float r, float g, float b) {
    glColor3f(r * dim, g * dim, b * dim);
    glBegin(GL_QUADS);
        glVertex2f(x, y);         glVertex2f(x + w, y);
        glVertex2f(x + w, y + h); glVertex2f(x, y + h);
    glEnd();
}

void drawCircle(float cx, float cy, float rad, float r, float g, float b) {
    glColor3f(r * dim, g * dim, b * dim);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int i = 0; i <= 360; i += 15) {
            float a = i * 3.14159265f / 180.0f;
            glVertex2f(cx + rad * cos(a), cy + rad * sin(a));
        }
    glEnd();
}

void drawText(float x, float y, const char* s, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (const char* p = s; *p; ++p)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *p);
}

// ---------------------------------------------------------------------
//  Environment: 2.5D Buildings, Park, and Side-View Trees
// ---------------------------------------------------------------------
void initBuildings() {
    buildings.clear();
    // Bottom-Left Block
    buildings.push_back({20, 20, 80, 100, 30, 0.6f, 0.6f, 0.7f});
    buildings.push_back({120, 20, 100, 120, 40, 0.7f, 0.5f, 0.5f});

    // Top-Right Block
    buildings.push_back({520, 420, 90, 80, 35, 0.6f, 0.5f, 0.6f});
    buildings.push_back({630, 420, 120, 140, 50, 0.4f, 0.5f, 0.7f});

    // Bottom-Right Block
    buildings.push_back({550, 40, 160, 100, 45, 0.55f, 0.55f, 0.55f});
}

void drawBuilding3D(const Building& b) {
    float R = b.r * dim, G = b.g * dim, B = b.b * dim;

    // Side face
    glColor3f(R * 0.6f, G * 0.6f, B * 0.6f);
    glBegin(GL_QUADS);
        glVertex2f(b.x + b.w, b.y);
        glVertex2f(b.x + b.w + b.d, b.y + b.d);
        glVertex2f(b.x + b.w + b.d, b.y + b.h + b.d);
        glVertex2f(b.x + b.w, b.y + b.h);
    glEnd();

    // Roof
    glColor3f(R * 0.8f, G * 0.8f, B * 0.8f);
    glBegin(GL_QUADS);
        glVertex2f(b.x, b.y + b.h);
        glVertex2f(b.x + b.w, b.y + b.h);
        glVertex2f(b.x + b.w + b.d, b.y + b.h + b.d);
        glVertex2f(b.x + b.d, b.y + b.h + b.d);
    glEnd();

    // Front face
    drawRect(b.x, b.y, b.w, b.h, b.r, b.g, b.b);

    // Windows
    for(float wy = b.y + 15; wy < b.y + b.h - 15; wy += 20) {
        for(float wx = b.x + 10; wx < b.x + b.w - 10; wx += 20) {
            float wr = isNight ? 0.9f : 0.2f;
            float wg = isNight ? 0.9f : 0.3f;
            float wb = isNight ? 0.6f : 0.4f;
            drawRect(wx, wy, 10, 12, wr, wg, wb);
        }
    }
}

void drawTreeSideView(float x, float y) {
    drawRect(x - 4, y, 8, 25, 0.4f, 0.2f, 0.1f);
    drawCircle(x, y + 35, 18, 0.1f, 0.45f, 0.15f);
    drawCircle(x - 12, y + 25, 14, 0.08f, 0.38f, 0.12f);
    drawCircle(x + 12, y + 25, 14, 0.12f, 0.5f, 0.18f);
}

void drawPark() {
    drawRect(0, 390, 290, 210, 0.25f, 0.55f, 0.25f);
    drawRect(120, 390, 40, 210, 0.7f, 0.6f, 0.5f);
    drawRect(0, 480, 120, 30, 0.7f, 0.6f, 0.5f);

    Building parkBldg = { 180, 450, 70, 50, 20, 0.8f, 0.7f, 0.6f };
    drawBuilding3D(parkBldg);

    drawTreeSideView(40, 420);
    drawTreeSideView(80, 530);
    drawTreeSideView(240, 410);
    drawTreeSideView(260, 540);
}

void drawRoads() {
    drawRect(0, 190, SCR_W, 220, 0.5f, 0.5f, 0.5f);
    drawRect(290, 0, 220, SCR_H, 0.5f, 0.5f, 0.5f);

    drawRect(0, 210, SCR_W, 180, 0.2f, 0.2f, 0.22f);
    drawRect(310, 0, 180, SCR_H, 0.2f, 0.2f, 0.22f);

    for (int y = 215; y < 385; y += 15) {
        drawRect(275, (float)y, 30, 8, 0.85f, 0.85f, 0.85f);
        drawRect(495, (float)y, 30, 8, 0.85f, 0.85f, 0.85f);
    }
    for (int x = 315; x < 485; x += 15) {
        drawRect((float)x, 175, 8, 30, 0.85f, 0.85f, 0.85f);
        drawRect((float)x, 395, 8, 30, 0.85f, 0.85f, 0.85f);
    }

    for (int x = 0; x < SCR_W; x += 40)
        if (x < 270 || x > 525) drawRect((float)x, 298, 20, 4, 1, 1, 1);
    for (int y = 0; y < SCR_H; y += 40)
        if (y < 170 || y > 425) drawRect(398, (float)y, 4, 20, 1, 1, 1);

    drawRect(0, 250, SCR_W, 3, 0.9f, 0.75f, 0.1f);
    drawRect(0, 350, SCR_W, 3, 0.9f, 0.75f, 0.1f);
    drawRect(350, 0, 3, SCR_H, 0.9f, 0.75f, 0.1f);
    drawRect(450, 0, 3, SCR_H, 0.9f, 0.75f, 0.1f);

    drawRect(310, 250, 6, 100, 1, 1, 1);
    drawRect(484, 250, 6, 100, 1, 1, 1);
    drawRect(350, 210, 100, 6, 1, 1, 1);
    drawRect(350, 384, 100, 6, 1, 1, 1);
}

// ---------------------------------------------------------------------
//  Traffic Lights
// ---------------------------------------------------------------------
LightState hState() { return (phase == 0) ? GREEN : (phase == 1 ? YELLOW : RED); }
LightState vState() { return (phase == 3) ? GREEN : (phase == 4 ? YELLOW : RED); }
const char* stateName(LightState s) { return s == GREEN ? "GREEN" : (s == YELLOW ? "YELLOW" : "RED"); }

void drawTrafficLight(float x, float y, LightState s) {
    drawRect(x, y, 16, 46, 0.1f, 0.1f, 0.1f);
    float R = (s == RED) ? 1 : 0.2f, G = (s == GREEN) ? 1 : 0.2f, Y = (s == YELLOW) ? 1 : 0.2f;
    float save = dim; dim = 1.0f;
    drawCircle(x + 8, y + 38, 5, R, 0.1f, 0.1f);
    drawCircle(x + 8, y + 23, 5, Y, Y*0.9f, 0.1f);
    drawCircle(x + 8, y + 8,  5, 0.1f, G, 0.2f);
    dim = save;
}

void drawScene() {
    drawRoads();
    drawPark();

    for (size_t i = 0; i < buildings.size(); i++)
        drawBuilding3D(buildings[i]);

    drawTreeSideView(280, 100);
    drawTreeSideView(780, 150);

    drawTrafficLight(290, 160, hState());
    drawTrafficLight(494, 394, hState());
    drawTrafficLight(290, 394, vState());
    drawTrafficLight(494, 160, vState());
}

// ---------------------------------------------------------------------
//  Vehicle Logic & Top-Down Rendering
// ---------------------------------------------------------------------
float gapAhead(const Vehicle& back, const Vehicle& front) {
    switch (back.dir) {
        case 0: return (front.x <= back.x) ? 1e9f : front.x - (back.x + vehLen(back.type));
        case 1: return (front.x >= back.x) ? 1e9f : back.x - (front.x + vehLen(front.type));
        case 2: return (front.y <= back.y) ? 1e9f : front.y - (back.y + vehLen(back.type));
        default:return (front.y >= back.y) ? 1e9f : (back.y - vehLen(back.type)) - front.y;
    }
}

bool mustStopAtLight(const Vehicle& v, float sp) {
    LightState s = (v.dir < 2) ? hState() : vState();
    if (s == GREEN) return false;
    float L = vehLen(v.type), f;
    switch (v.dir) {
        case 0: f = v.x + L; return f <= STOP_R && f + sp > STOP_R;
        case 1: f = v.x;     return f >= STOP_L && f - sp < STOP_L;
        case 2: f = v.y + L; return f <= STOP_U && f + sp > STOP_U;
        default:f = v.y - L; return f >= STOP_D && f - sp < STOP_D;
    }
}

bool tooClose(int i) {
    for (int j = 0; j < (int)vehicles.size(); j++) {
        if (j == i || vehicles[j].dir != vehicles[i].dir) continue;
        if (vehicles[i].type != AMBULANCE && vehicles[j].type == AMBULANCE) continue;
        if (vehicles[i].type == AMBULANCE && vehicles[j].type != AMBULANCE) continue;
        if (gapAhead(vehicles[i], vehicles[j]) < GAP) return true;
    }
    return false;
}

Vehicle makeVehicle(int type, int dir, float x, float y) {
    Vehicle v; v.type = type; v.dir = dir; v.x = x; v.y = y; v.remove = false;
    static const float bCol[4][3]={{0.9f,0.7f,0.1f},{0.1f,0.5f,0.8f},{0.8f,0.2f,0.2f},{0.1f,0.7f,0.4f}};
    static const float tCol[4][3]={{0.7f,0.1f,0.1f},{0.2f,0.3f,0.7f},{0.8f,0.5f,0.1f},{0.3f,0.3f,0.3f}};
    int k = rand() % 4;

    if (type == BUS) { v.speed = 1.2f + (rand()%5)/10.f; v.r=bCol[k][0]; v.g=bCol[k][1]; v.b=bCol[k][2]; }
    else if (type == TRUCK) { v.speed = 1.3f + (rand()%6)/10.f; v.r=tCol[k][0]; v.g=tCol[k][1]; v.b=tCol[k][2]; }
    else if (type == AMBULANCE) { v.speed = 3.2f; v.r=1; v.g=1; v.b=1; }
    else { v.speed = 1.5f + (rand()%11)/10.f; v.r=0.2f+0.8f*rnd01(); v.g=0.2f+0.8f*rnd01(); v.b=0.2f+0.8f*rnd01(); }
    return v;
}

bool spawnVehicle(int type, int side) {
    int amb = 0; for (size_t i = 0; i < vehicles.size(); i++) if (vehicles[i].type == AMBULANCE) amb++;
    if (type == AMBULANCE && amb >= MAX_AMBULANCES) return false;
    if (type != AMBULANCE && (int)vehicles.size() >= MAX_VEHICLES) return false;

    float L = vehLen(type), W = vehW(type);
    int start = (side >= 0) ? side : rand() % 4;
    int tries = (side >= 0) ? 1 : 4;

    for (int k = 0; k < tries; k++) {
        int d = (start + k) % 4;
        float x = 0, y = 0;
        if (d == 0)      { x = -L;       y = (type == AMBULANCE) ? 230 - W/2 : 275 - W/2; }
        else if (d == 1) { x = (float)SCR_W; y = (type == AMBULANCE) ? 370 - W/2 : 325 - W/2; }
        else if (d == 2) { x = (type == AMBULANCE) ? 470 + W/2 : 425 + W/2; y = -L; }
        else             { x = (type == AMBULANCE) ? 330 - W/2 : 375 - W/2; y = SCR_H + L; }

        Vehicle n = makeVehicle(type, d, x, y);
        bool free = true;
        for (size_t j = 0; j < vehicles.size(); j++) {
            if (vehicles[j].dir != d || vehicles[j].type != type) continue;
            if (gapAhead(n, vehicles[j]) < GAP || gapAhead(vehicles[j], n) < GAP) free = false;
        }
        if (free) { vehicles.push_back(n); return true; }
    }
    return false;
}

void initVehicles() {
    vehicles.clear();

    // Exactly 5 vehicles initially
    spawnVehicle(CAR, 0);   // Move Right
    spawnVehicle(BUS, 1);   // Move Left
    spawnVehicle(TRUCK, 2); // Move Up
    spawnVehicle(CAR, 3);   // Move Down
    spawnVehicle(CAR, 0);   // Extra Car
}

void drawVehicleTopDown(const Vehicle& v) {
    float L = vehLen(v.type), W = vehW(v.type);
    glPushMatrix();
    glTranslatef(v.x, v.y, 0);
    if (v.dir == 1) { glTranslatef(L, 0, 0); glScalef(-1, 1, 1); }
    else if (v.dir == 2) { glRotatef(90, 0, 0, 1); }
    else if (v.dir == 3) { glRotatef(-90, 0, 0, 1); }

    if (v.type == BUS) {
        drawRect(0, 0, L, W, v.r, v.g, v.b);
        drawRect(L-4, 2, 4, W-4, 0.1f, 0.1f, 0.1f);
    } else if (v.type == TRUCK) {
        drawRect(0, 0, 65, W, 0.8f, 0.8f, 0.8f);
        drawRect(67, 2, 23, W-4, v.r, v.g, v.b);
        drawRect(86, 3, 4, W-6, 0.1f, 0.1f, 0.1f);
    } else if (v.type == AMBULANCE) {
        drawRect(0, 0, L, W, 1, 1, 1);
        drawRect(L-12, 2, 6, W-4, 0.1f, 0.1f, 0.1f);
        drawRect(25, W/2-2, 16, 4, 0.8f, 0.1f, 0.1f);
        drawRect(31, W/2-8, 4, 16, 0.8f, 0.1f, 0.1f);
        bool flash = (glutGet(GLUT_ELAPSED_TIME)/150)%2 == 0;
        float save=dim; dim=1.0f;
        drawRect(50, 2, 4, 8, flash?1:0.2f, 0.1f, flash?0.1f:1);
        drawRect(50, W-10, 4, 8, flash?0.1f:1, 0.1f, flash?1:0.1f);
        dim=save;
    } else { // CAR
        drawRect(0, 0, L, W, v.r, v.g, v.b);
        drawRect(14, 2, 28, W-4, v.r*0.6f, v.g*0.6f, v.b*0.6f);
        drawRect(42, 3, 6, W-6, 0.1f, 0.1f, 0.2f);

        float save = dim; dim = 1.0f;
        drawRect(L-2, 2, 2, 5, 1, 1, 0.8f);
        drawRect(L-2, W-7, 2, 5, 1, 1, 0.8f);
        drawRect(0, 2, 2, 5, 0.8f, 0.1f, 0.1f);
        drawRect(0, W-7, 2, 5, 0.8f, 0.1f, 0.1f);
        dim = save;
    }

    if (isNight && v.type != AMBULANCE) {
        float save = dim; dim = 1.0f;
        glColor4f(1.0f, 1.0f, 0.6f, 0.6f);
        glBegin(GL_TRIANGLES);
            glVertex2f(L, W - 6);
            glVertex2f(L + 60, W + 10);
            glVertex2f(L + 60, W - 20);

            glVertex2f(L, 6);
            glVertex2f(L + 60, 20);
            glVertex2f(L + 60, -10);
        glEnd();
        dim = save;
    }

    glPopMatrix();
}

// ---------------------------------------------------------------------
//  Update Loop & HUD
// ---------------------------------------------------------------------
bool ambActive(const Vehicle& a) {
    float L = vehLen(a.type);
    if (a.dir==0) return a.x <= 490; if (a.dir==1) return a.x+L >= 310;
    if (a.dir==2) return a.y <= 390; return a.y >= 210;
}
int findEmerg() {
    for (size_t i=0; i<vehicles.size(); i++)
        if (vehicles[i].type==AMBULANCE && ambActive(vehicles[i])) return vehicles[i].dir<2 ? 0:1;
    return -1;
}

void update(int value) {
    if (!paused) {
        emergAxis = findEmerg();
        phaseTime += 0.016f * speedMul;
        int myG = 0, othG = 3; if (emergAxis == 1) { myG = 3; othG = 0; }
        if (emergAxis != -1) {
            if (phase == myG) phaseTime = 0;
            else if (phase == othG) { phase = othG + 1; phaseTime = 0; }
        }
        if (phaseTime >= PHASE_DUR[phase]) {
            phaseTime = 0; int next = (phase + 1) % 6;
            if (emergAxis != -1 && next == othG) next = myG;
            phase = next;
        }

        for (int i = 0; i < (int)vehicles.size(); i++) {
            Vehicle& v = vehicles[i]; float sp = v.speed * speedMul;
            if (mustStopAtLight(v, sp) || tooClose(i)) continue;
            if (v.dir==0) v.x+=sp; else if (v.dir==1) v.x-=sp; else if (v.dir==2) v.y+=sp; else v.y-=sp;

            float L = vehLen(v.type);
            bool out = (v.dir==0&&v.x>SCR_W) || (v.dir==1&&v.x<-L) || (v.dir==2&&v.y>SCR_H) || (v.dir==3&&v.y<-L);
            if (out) {
                if (v.type == AMBULANCE) v.remove = true;
                else {
                    if (v.dir==0) v.x=-L; if (v.dir==1) v.x=(float)SCR_W;
                    if (v.dir==2) v.y=-L; if (v.dir==3) v.y=SCR_H+L;
                }
            }
        }

        for (int i = (int)vehicles.size()-1; i >= 0; i--)
            if (vehicles[i].remove) { vehicles.erase(vehicles.begin()+i); }
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void drawHUD() {
    float save = dim; dim = 1.0f;
    glColor4f(0.1f, 0.15f, 0.2f, 0.9f);
    glBegin(GL_QUADS);
        glVertex2f(0, 545); glVertex2f(SCR_W, 545);
        glVertex2f(SCR_W, SCR_H); glVertex2f(0, SCR_H);
    glEnd();

    int c[4] = {0,0,0,0}; for (size_t i=0; i<vehicles.size(); i++) c[vehicles[i].type]++;
    char buf[220];

    // Crash counter removed from HUD format string
    snprintf(buf, sizeof(buf), "H: %s | V: %s | Car %d Bus %d Truck %d Amb %d | Spd %.2fx%s",
             stateName(hState()), stateName(vState()), c[CAR], c[BUS], c[TRUCK], c[AMBULANCE], speedMul, paused?" | PAUSED":"");

    drawText(15, 582, buf, 1, 1, 1);
    drawText(15, 565, "C car | B bus | T truck | E ambulance | SPACE pause | +/- speed", 0.7f, 0.9f, 1.0f);
    drawText(15, 550, "Arrows: side | N day/night | R reset | ESC exit", 0.7f, 0.9f, 1.0f);

    if (isNight) { drawCircle(775, 575, 12, 0.8f, 0.8f, 0.8f); drawCircle(780, 578, 10, 0.1f, 0.15f, 0.2f); }
    else { drawCircle(775, 575, 12, 1, 0.9f, 0.2f); }

    if (emergAxis != -1) {
        bool flash = (glutGet(GLUT_ELAPSED_TIME)/300)%2==0;
        drawRect(140, 520, 520, 24, flash?0.8f:0.1f, 0.1f, flash?0.1f:0.8f);
        char msg[120]; snprintf(msg, sizeof(msg), "EMERGENCY: %s ROAD CLEARED", emergAxis==0?"HORIZ":"VERT");
        drawText(270, 527, msg, 1, 1, 1);
    }
    if (glutGet(GLUT_ELAPSED_TIME) < msgUntil) drawText(15, 500, msgText, 1, 0.8f, 0.2f);
    dim = save;
}

void display() {
    dim = isNight ? 0.35f : 1.0f;
    glClearColor(0.25f * dim, 0.5f * dim, 0.25f * dim, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    drawScene();
    for (size_t i = 0; i < vehicles.size(); i++)
        if (vehicles[i].type != AMBULANCE) drawVehicleTopDown(vehicles[i]);
    for (size_t i = 0; i < vehicles.size(); i++)
        if (vehicles[i].type == AMBULANCE) drawVehicleTopDown(vehicles[i]);

    drawHUD();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27:  exit(0);
        case ' ': paused = !paused; break;
        case '+': case '=': speedMul += 0.25f; if (speedMul>3) speedMul=3; break;
        case '-': case '_': speedMul -= 0.25f; if (speedMul<0.25f) speedMul=0.25f; break;
        case 'a': case 'A': spawnVehicle(rand()%3, selSide); break;
        case 'c': case 'C': spawnVehicle(CAR,       selSide); break;
        case 'b': case 'B': spawnVehicle(BUS,       selSide); break;
        case 't': case 'T': spawnVehicle(TRUCK,     selSide); break;
        case 'e': case 'E': spawnVehicle(AMBULANCE, selSide); break;
        case '1': selSide=0; break; case '2': selSide=1; break;
        case '3': selSide=2; break; case '4': selSide=3; break;
        case 'z': case 'Z': selSide=-1; break;
        case 'n': case 'N': isNight = !isNight; break;
        case 'r': case 'R': initVehicles(); break;
    }
}
void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT: selSide=0; break; case GLUT_KEY_RIGHT: selSide=1; break;
        case GLUT_KEY_UP:   selSide=2; break; case GLUT_KEY_DOWN:  selSide=3; break;
    }
}
void reshape(int w, int h) { glViewport(0, 0, w, h); }
void init() {
    initBuildings();
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluOrtho2D(0, SCR_W, 0, SCR_H);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char** argv) {
    srand((unsigned)time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // Large Windowed Mode (1600 x 900)
    glutInitWindowSize(1600, 900);
    glutCreateWindow("2D Traffic Simulator");

    // glutFullScreen(); // Disabled full screen

    init();
    initVehicles();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}
