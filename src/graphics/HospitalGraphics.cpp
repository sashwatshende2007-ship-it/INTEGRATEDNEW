#include "graphics/HospitalGraphics.h"
#include "utilities/Utils.h"

#include <iostream>
#include <cmath>
#include <sstream>
#include <cstdio>
#include <cstring>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

HospitalGraphics* HospitalGraphics::instance = nullptr;

HospitalGraphics* HospitalGraphics::getInstance() {
    if (!instance) {
        instance = new HospitalGraphics();
    }
    return instance;
}

HospitalGraphics::HospitalGraphics()
    : hospitalRef(nullptr), windowWidth(1280), windowHeight(720),
      currentMode(DASHBOARD_MODE), windowTitle("Smart Hospital Management System - Visualization (16:9 HD)"),
      isRunning(false), activePanelId(0) {
    initButtons();
    setupPathWaypoints();
}

void HospitalGraphics::setupPathWaypoints() {
    cart.path.clear();
    // Path from Emergency Room to ICU Bed #19 in 1280x720 16:9 layout
    cart.path.push_back({150.0f, 495.0f}); // 0: ER Trauma Bay
    cart.path.push_back({380.0f, 495.0f}); // 1: ER Corridor Exit
    cart.path.push_back({555.0f, 495.0f}); // 2: Main Central Corridor Junction
    cart.path.push_back({555.0f, 320.0f}); // 3: Vertical Hallway
    cart.path.push_back({555.0f, 175.0f}); // 4: ICU Entry Junction
    cart.path.push_back({850.0f, 175.0f}); // 5: ICU Corridor
    cart.path.push_back({940.0f, 175.0f}); // 6: Target ICU Bed #19
}

void HospitalGraphics::initButtons() {
    buttons.clear();
    float startX = 60.0f;
    float startY = 175.0f;
    float btnW = 260.0f;
    float btnH = 48.0f;
    float gapY = 62.0f;

    // Left Column Management Buttons
    buttons.push_back({startX, startY + 0 * gapY, btnW, btnH, "1. Patient Records", 1, false});
    buttons.push_back({startX, startY + 1 * gapY, btnW, btnH, "2. Doctor Roster", 2, false});
    buttons.push_back({startX, startY + 2 * gapY, btnW, btnH, "3. Appointments", 3, false});
    buttons.push_back({startX, startY + 3 * gapY, btnW, btnH, "4. Ward Beds", 4, false});
    buttons.push_back({startX, startY + 4 * gapY, btnW, btnH, "5. Pharmacy Stock", 5, false});
    buttons.push_back({startX, startY + 5 * gapY, btnW, btnH, "6. Billing & Invoices", 6, false});

    // Middle Column Action / Simulation Buttons
    float midX = 345.0f;
    buttons.push_back({midX, startY + 0 * gapY, btnW, btnH, "7. Top-Down Hospital View", 10, false});
    buttons.push_back({midX, startY + 1 * gapY, btnW, btnH, "8. Emergency Cart Simulation", 11, false});
    buttons.push_back({midX, startY + 2 * gapY, btnW, btnH, "9. 80386 Hardware Billing", 9, false});
    buttons.push_back({midX, startY + 3 * gapY, btnW, btnH, "10. Data Structures Demo", 12, false});
    buttons.push_back({midX, startY + 4 * gapY, btnW, btnH, "11. Hospital Status Overview", 13, false});
    buttons.push_back({midX, startY + 5 * gapY, btnW, btnH, "12. Return to Dashboard", 0, false});
}

// ============================================================================
// CG CO1: Drawing Primitives (GL_QUADS, GL_LINE_LOOP, GL_TRIANGLE_FAN, GL_LINES)
// ============================================================================
void HospitalGraphics::drawRectangle(float x, float y, float w, float h, const GLColor& color, bool filled) {
    glColor4f(color.r, color.g, color.b, color.a);
    if (filled) {
        glBegin(GL_QUADS);
    } else {
        glBegin(GL_LINE_LOOP);
    }
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void HospitalGraphics::drawCircle(float cx, float cy, float radius, const GLColor& color, int segments, bool filled) {
    glColor4f(color.r, color.g, color.b, color.a);
    if (filled) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy); // Center vertex
    } else {
        glBegin(GL_LINE_LOOP);
    }
    for (int i = 0; i <= segments; ++i) {
        float angle = static_cast<float>(i * 2.0 * Utils::PI / segments);
        float vx = cx + std::cos(angle) * radius;
        float vy = cy + std::sin(angle) * radius;
        glVertex2f(vx, vy);
    }
    glEnd();
}

void HospitalGraphics::drawLine(float x1, float y1, float x2, float y2, const GLColor& color, float lineWidth) {
    glLineWidth(lineWidth);
    glColor4f(color.r, color.g, color.b, color.a);
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
    glLineWidth(1.0f);
}

#ifdef _WIN32
static HFONT g_hFont = nullptr;
static GLuint g_fontBase = 0;

void initWin32Font(HDC hdc) {
    if (!g_fontBase) {
        g_fontBase = glGenLists(96);
        g_hFont = CreateFontA(
            -16, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Segoe UI"
        );
        SelectObject(hdc, g_hFont);
        wglUseFontBitmapsA(hdc, 32, 96, g_fontBase);
    }
}
#endif

void HospitalGraphics::drawText(float x, float y, const char* text, const GLColor& color) {
    if (!text || text[0] == '\0') return;
    glColor4f(color.r, color.g, color.b, color.a);
    glRasterPos2f(x, y);
#ifdef _WIN32
    if (g_fontBase) {
        glPushAttrib(GL_LIST_BIT);
        glListBase(g_fontBase - 32);
        glCallLists(static_cast<GLsizei>(strlen(text)), GL_UNSIGNED_BYTE, text);
        glPopAttrib();
    }
#endif
}

// ============================================================================
// CG CO3: 2D Object Construction Algorithms (Bed, Chair, Cart, Cross, Rooms)
// ============================================================================
void HospitalGraphics::drawMedicalCross(float x, float y, float size, const GLColor& color) {
    float armW = size * 0.333333f;
    float half = size * 0.5f;
    float halfArm = armW * 0.5f;

    glColor4f(color.r, color.g, color.b, color.a);
    glBegin(GL_QUADS);
    // Vertical bar
    glVertex2f(x - halfArm, y - half);
    glVertex2f(x + halfArm, y - half);
    glVertex2f(x + halfArm, y + half);
    glVertex2f(x - halfArm, y + half);

    // Horizontal bar
    glVertex2f(x - half, y - halfArm);
    glVertex2f(x + half, y - halfArm);
    glVertex2f(x + half, y + halfArm);
    glVertex2f(x - half, y + halfArm);
    glEnd();
}

void HospitalGraphics::drawBed(float x, float y, bool isOccupied, int bedNumber) {
    float w = 48.0f;
    float h = 68.0f;

    // Bed Outer Frame (Wood/Metal)
    drawRectangle(x, y, w, h, GLColor(0.25f, 0.28f, 0.32f), true);
    drawRectangle(x, y, w, h, GLColor(0.1f, 0.1f, 0.1f), false);

    // Mattress
    drawRectangle(x + 3.0f, y + 3.0f, w - 6.0f, h - 6.0f, GLColor(0.92f, 0.94f, 0.96f), true);

    // Pillow
    drawRectangle(x + 8.0f, y + 6.0f, w - 16.0f, 14.0f, GLColor(1.0f, 1.0f, 1.0f), true);
    drawRectangle(x + 8.0f, y + 6.0f, w - 16.0f, 14.0f, GLColor(0.7f, 0.7f, 0.7f), false);

    // Blanket (Green if Available, Red/Navy if Occupied)
    GLColor blanketColor = isOccupied ? GLColor(0.85f, 0.25f, 0.25f) : GLColor(0.25f, 0.75f, 0.40f);
    drawRectangle(x + 5.0f, y + 24.0f, w - 10.0f, h - 28.0f, blanketColor, true);

    if (isOccupied) {
        // Patient Head Circle
        drawCircle(x + w / 2.0f, y + 13.0f, 5.0f, GLColor(0.95f, 0.80f, 0.65f), 16, true);
    }

    // Bed Number Label
    std::string bLabel = "#" + std::to_string(bedNumber);
    drawText(x + 8.0f, y + 54.0f, bLabel, GLColor(1.0f, 1.0f, 1.0f));
}

void HospitalGraphics::drawChair(float x, float y) {
    float size = 18.0f;
    // Seat cushion
    drawRectangle(x, y, size, size, GLColor(0.2f, 0.4f, 0.7f), true);
    // Backrest
    drawRectangle(x, y - 5.0f, size, 5.0f, GLColor(0.15f, 0.3f, 0.55f), true);
    // Outline
    drawRectangle(x, y - 5.0f, size, size + 5.0f, GLColor(0.0f, 0.0f, 0.0f), false);
}

void HospitalGraphics::drawPatientCart() {
    float cw = 50.0f;
    float ch = 28.0f;

    // Cart Metallic Chassis
    drawRectangle(-cw / 2.0f, -ch / 2.0f, cw, ch, GLColor(0.75f, 0.78f, 0.82f), true);
    drawRectangle(-cw / 2.0f, -ch / 2.0f, cw, ch, GLColor(0.2f, 0.2f, 0.2f), false);

    // Mattress & Stretcher
    drawRectangle(-cw / 2.0f + 4.0f, -ch / 2.0f + 3.0f, cw - 8.0f, ch - 6.0f, GLColor(0.95f, 0.95f, 0.98f), true);

    // Patient on Cart
    // Patient Body Blanket
    drawRectangle(-cw / 2.0f + 16.0f, -ch / 2.0f + 5.0f, cw - 22.0f, ch - 10.0f, GLColor(0.2f, 0.5f, 0.9f), true);
    // Patient Head (Circle)
    drawCircle(-cw / 2.0f + 10.0f, 0.0f, 5.5f, GLColor(0.95f, 0.80f, 0.65f), 16, true);

    // Red Emergency Medical Cross on Cart
    drawMedicalCross(cw / 2.0f - 14.0f, 0.0f, 10.0f, GLColor(0.9f, 0.1f, 0.1f));

    // 4 Wheels (Parametric Circles with GL_TRIANGLE_FAN)
    float wheelR = 4.5f;
    drawCircle(-cw / 2.0f + 6.0f, -ch / 2.0f - 3.0f, wheelR, GLColor(0.2f, 0.2f, 0.2f), 16, true);
    drawCircle( cw / 2.0f - 6.0f, -ch / 2.0f - 3.0f, wheelR, GLColor(0.2f, 0.2f, 0.2f), 16, true);
    drawCircle(-cw / 2.0f + 6.0f,  ch / 2.0f + 3.0f, wheelR, GLColor(0.2f, 0.2f, 0.2f), 16, true);
    drawCircle( cw / 2.0f - 6.0f,  ch / 2.0f + 3.0f, wheelR, GLColor(0.2f, 0.2f, 0.2f), 16, true);
}

void HospitalGraphics::drawRoom(float x, float y, float w, float h, const std::string& title, const GLColor& floorColor) {
    // Floor
    drawRectangle(x, y, w, h, floorColor, true);

    // Walls (Outer Border)
    glLineWidth(3.0f);
    drawRectangle(x, y, w, h, GLColor(0.2f, 0.25f, 0.3f), false);
    glLineWidth(1.0f);

    // Room Header Bar
    drawRectangle(x, y, w, 24.0f, GLColor(0.25f, 0.32f, 0.42f), true);
    drawText(x + 10.0f, y + 17.0f, title, GLColor(1.0f, 1.0f, 1.0f));
}

void HospitalGraphics::drawCorridors() {
    GLColor corridorFloor(0.90f, 0.90f, 0.92f);
    // Main Horizontal Corridor
    drawRectangle(50.0f, 460.0f, 1180.0f, 75.0f, corridorFloor, true);
    // Vertical Hallway to ICU & Surgery
    drawRectangle(510.0f, 130.0f, 90.0f, 335.0f, corridorFloor, true);
    // ICU Horizontal Corridor
    drawRectangle(600.0f, 145.0f, 630.0f, 65.0f, corridorFloor, true);

    // Corridor dashed center guidelines
    for (float cx = 60.0f; cx < 1220.0f; cx += 35.0f) {
        drawLine(cx, 497.0f, cx + 18.0f, 497.0f, GLColor(0.75f, 0.75f, 0.78f), 2.0f);
    }
    for (float cy = 140.0f; cy < 460.0f; cy += 35.0f) {
        drawLine(555.0f, cy, 555.0f, cy + 18.0f, GLColor(0.75f, 0.75f, 0.78f), 2.0f);
    }
}

// ============================================================================
// CG CO2 & CO4: Projection and Transformations (Cart Movement & Viewing)
// ============================================================================
void HospitalGraphics::setupOrthographicProjection() {
    glViewport(0, 0, windowWidth, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Top-Left origin: (0, 0) top-left, (windowWidth, windowHeight) bottom-right
    gluOrtho2D(0.0, static_cast<double>(windowWidth), static_cast<double>(windowHeight), 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
}

void HospitalGraphics::applyCartTransformations() {
    glPushMatrix();
    // Translation: Move cart along hospital corridor path
    glTranslatef(cart.x, cart.y, 0.0f);
    // Rotation: Turn 90 degrees at corridor corners
    glRotatef(cart.angle, 0.0f, 0.0f, 1.0f);
    // Scaling: Zoom cart for clear visualization
    glScalef(cart.scale, cart.scale, 1.0f);

    drawPatientCart();
    glPopMatrix();
}

void HospitalGraphics::updateCartAnimation() {
    if (!cart.isMoving || cart.currentWayPoint >= cart.path.size()) return;

    auto target = cart.path[cart.currentWayPoint];
    float dx = target.first - cart.x;
    float dy = target.second - cart.y;
    float dist = std::sqrt(dx * dx + dy * dy);
    float speed = 2.5f;

    if (dist < speed) {
        cart.x = target.first;
        cart.y = target.second;
        cart.currentWayPoint++;

        if (cart.currentWayPoint < cart.path.size()) {
            auto nextT = cart.path[cart.currentWayPoint];
            float ndx = nextT.first - cart.x;
            float ndy = nextT.second - cart.y;
            cart.angle = static_cast<float>(std::atan2(ndy, ndx) * 180.0 / Utils::PI);
        } else {
            cart.isMoving = false; // Reached destination
        }
    } else {
        cart.x += (dx / dist) * speed;
        cart.y += (dy / dist) * speed;
        cart.angle = static_cast<float>(std::atan2(dy, dx) * 180.0 / Utils::PI);
    }
}

// ============================================================================
// Mode Renderers
// ============================================================================
void HospitalGraphics::renderDashboardGUI() {
    // Background
    drawRectangle(0.0f, 0.0f, windowWidth, windowHeight, GLColor(0.94f, 0.95f, 0.97f), true);

    // Top Navigation Banner (16:9 HD Header)
    drawRectangle(0.0f, 0.0f, windowWidth, 80.0f, GLColor(0.12f, 0.22f, 0.36f), true);
    drawMedicalCross(45.0f, 40.0f, 32.0f, GLColor(0.9f, 0.2f, 0.2f));
    drawText(80.0f, 48.0f, "SMART HOSPITAL MANAGEMENT SYSTEM", GLColor(1.0f, 1.0f, 1.0f));
    drawText(windowWidth - 480.0f, 48.0f, "MIT ACADEMY OF ENGINEERING (PSOOP | DS | COA | CG)", GLColor(0.8f, 0.88f, 0.98f));

    // Stats Cards at Top (4 cards distributed across 1280px)
    float cardW = 270.0f;
    float cardH = 55.0f;
    float cardY = 95.0f;

    size_t pCnt = hospitalRef ? hospitalRef->getTotalPatients() : 4;
    size_t dCnt = hospitalRef ? hospitalRef->getTotalDoctors() : 5;
    size_t bCnt = hospitalRef ? hospitalRef->getTotalBeds() : 25;
    size_t emgCnt = hospitalRef ? hospitalRef->getEmergencyCount() : 1;

    // Card 1: Patients (Clickable -> Panel 1)
    drawRectangle(60.0f, cardY, cardW, cardH, (activePanelId == 1) ? GLColor(0.25f, 0.6f, 0.95f) : GLColor(0.2f, 0.5f, 0.8f), true);
    drawRectangle(60.0f, cardY, cardW, cardH, (activePanelId == 1) ? GLColor(1.0f, 1.0f, 1.0f) : GLColor(0.15f, 0.35f, 0.6f), false);
    drawText(75.0f, cardY + 24.0f, "TOTAL PATIENTS [Click]", GLColor(0.85f, 0.92f, 1.0f));
    drawText(75.0f, cardY + 46.0f, std::to_string(pCnt) + " Registered", GLColor(1.0f, 1.0f, 1.0f));

    // Card 2: Doctors (Clickable -> Panel 2)
    drawRectangle(350.0f, cardY, cardW, cardH, (activePanelId == 2) ? GLColor(0.25f, 0.75f, 0.6f) : GLColor(0.2f, 0.65f, 0.5f), true);
    drawRectangle(350.0f, cardY, cardW, cardH, (activePanelId == 2) ? GLColor(1.0f, 1.0f, 1.0f) : GLColor(0.15f, 0.45f, 0.35f), false);
    drawText(365.0f, cardY + 24.0f, "ACTIVE DOCTORS [Click]", GLColor(0.85f, 1.0f, 0.92f));
    drawText(365.0f, cardY + 46.0f, std::to_string(dCnt) + " On Duty", GLColor(1.0f, 1.0f, 1.0f));

    // Card 3: Beds (Clickable -> Panel 4)
    drawRectangle(640.0f, cardY, cardW, cardH, (activePanelId == 4) ? GLColor(0.95f, 0.65f, 0.25f) : GLColor(0.85f, 0.55f, 0.15f), true);
    drawRectangle(640.0f, cardY, cardW, cardH, (activePanelId == 4) ? GLColor(1.0f, 1.0f, 1.0f) : GLColor(0.6f, 0.35f, 0.1f), false);
    drawText(655.0f, cardY + 24.0f, "WARD CAPACITY [Click]", GLColor(1.0f, 0.95f, 0.85f));
    drawText(655.0f, cardY + 46.0f, std::to_string(bCnt) + " Total Beds", GLColor(1.0f, 1.0f, 1.0f));

    // Card 4: Emergency (Clickable -> Mode 2 Emergency Simulation)
    drawRectangle(930.0f, cardY, cardW, cardH, GLColor(0.85f, 0.25f, 0.25f), true);
    drawRectangle(930.0f, cardY, cardW, cardH, GLColor(1.0f, 0.8f, 0.2f), false);
    drawText(945.0f, cardY + 24.0f, "EMERGENCY TRIAGE [Sim]", GLColor(1.0f, 0.85f, 0.85f));
    drawText(945.0f, cardY + 46.0f, std::to_string(emgCnt) + " Critical (Click to Sim)", GLColor(1.0f, 1.0f, 1.0f));

    // Render Clickable Buttons (2 Columns)
    for (const auto& btn : buttons) {
        bool isActive = (btn.actionId == activePanelId);
        GLColor btnCol;
        if (isActive) {
            btnCol = GLColor(0.20f, 0.48f, 0.80f);
        } else if (btn.isHovered) {
            btnCol = GLColor(0.28f, 0.45f, 0.68f);
        } else {
            btnCol = GLColor(0.18f, 0.32f, 0.50f);
        }
        drawRectangle(btn.x, btn.y, btn.width, btn.height, btnCol, true);
        if (isActive) {
            drawRectangle(btn.x, btn.y, btn.width, btn.height, GLColor(0.35f, 0.80f, 1.0f), false);
            drawRectangle(btn.x - 1.0f, btn.y - 1.0f, btn.width + 2.0f, btn.height + 2.0f, GLColor(0.35f, 0.80f, 1.0f), false);
        } else {
            drawRectangle(btn.x, btn.y, btn.width, btn.height, GLColor(0.1f, 0.15f, 0.25f), false);
        }
        std::string labelText = (isActive ? "> " : "  ") + btn.label;
        drawText(btn.x + 10.0f, btn.y + 29.0f, labelText.c_str(), GLColor(1.0f, 1.0f, 1.0f));
    }

    // Status / System Information Panel on Right
    float infoX = 640.0f;
    float infoY = 175.0f;
    float infoW = 580.0f;
    float infoH = 485.0f;
    drawRectangle(infoX, infoY, infoW, infoH, GLColor(1.0f, 1.0f, 1.0f), true);
    drawRectangle(infoX, infoY, infoW, infoH, GLColor(0.75f, 0.80f, 0.85f), false);
    drawRectangle(infoX, infoY, infoW, 40.0f, GLColor(0.22f, 0.32f, 0.45f), true);

    switch (activePanelId) {
        case 1: { // Patient Records
            drawText(infoX + 20.0f, infoY + 26.0f, "PATIENT ADMISSION DIRECTORY (PSOOP CO1, CO4)", GLColor(1.0f, 1.0f, 1.0f));
            drawText(infoX + 20.0f, infoY + 65.0f, "ID       NAME                AGE  BLOOD  CONDITION", GLColor(0.35f, 0.45f, 0.55f));
            drawText(infoX + 410.0f, infoY + 65.0f, "BED", GLColor(0.35f, 0.45f, 0.55f));
            drawLine(infoX + 20.0f, infoY + 74.0f, infoX + infoW - 20.0f, infoY + 74.0f, GLColor(0.80f, 0.85f, 0.90f), 1.5f);

            float rowY = infoY + 98.0f;
            if (hospitalRef && !hospitalRef->getAllPatients().empty()) {
                int count = 0;
                for (const auto& pair : hospitalRef->getAllPatients()) {
                    if (count++ >= 8) break;
                    const Patient& p = pair.second;
                    std::string bedStr = (p.getAssignedBedId() > 0) ? ("Bed #" + std::to_string(p.getAssignedBedId())) : "None";
                    char rowBuf[128];
                    std::string nameSub = p.getName().substr(0, 16);
                    snprintf(rowBuf, sizeof(rowBuf), "%-6s  %-18s  %-3d  %-5s  %-18s",
                             p.getId().c_str(), nameSub.c_str(), p.getAge(), p.getBloodGroup().c_str(),
                             p.getMedicalCondition().substr(0, 18).c_str());
                    drawText(infoX + 20.0f, rowY, rowBuf, GLColor(0.15f, 0.20f, 0.28f));
                    drawText(infoX + 410.0f, rowY, bedStr.c_str(), p.getAssignedBedId() > 0 ? GLColor(0.1f, 0.55f, 0.2f) : GLColor(0.6f, 0.6f, 0.6f));
                    rowY += 32.0f;
                }
            } else {
                drawText(infoX + 20.0f, rowY, "No registered patients found.", GLColor(0.4f, 0.4f, 0.4f));
            }
            drawRectangle(infoX, infoY + infoH - 45.0f, infoW, 45.0f, GLColor(0.95f, 0.97f, 1.0f), true);
            drawText(infoX + 20.0f, infoY + infoH - 18.0f, "OOP Concepts: Person Base Class Inheritance, Blood Validation, O(1) Lookups", GLColor(0.2f, 0.4f, 0.65f));
            break;
        }
        case 2: { // Doctor Roster
            drawText(infoX + 20.0f, infoY + 26.0f, "DOCTOR ROSTER & SPECIALISTS (PSOOP CO1)", GLColor(1.0f, 1.0f, 1.0f));
            drawText(infoX + 20.0f, infoY + 65.0f, "ID       NAME                SPECIALTY         DEPT        FEE      LOAD", GLColor(0.35f, 0.45f, 0.55f));
            drawLine(infoX + 20.0f, infoY + 74.0f, infoX + infoW - 20.0f, infoY + 74.0f, GLColor(0.80f, 0.85f, 0.90f), 1.5f);

            float rowY = infoY + 98.0f;
            if (hospitalRef && !hospitalRef->getAllDoctors().empty()) {
                int count = 0;
                for (const auto& pair : hospitalRef->getAllDoctors()) {
                    if (count++ >= 8) break;
                    const Doctor& d = pair.second;
                    char rowBuf[128];
                    std::string nameSub = d.getName().substr(0, 16);
                    snprintf(rowBuf, sizeof(rowBuf), "%-6s  %-16s  %-14s  %-9s  Rs %-4.0f  %d/%d",
                             d.getId().c_str(), nameSub.c_str(), d.getSpecialization().substr(0, 14).c_str(),
                             d.getDepartment().substr(0, 9).c_str(), d.getConsultationFee(),
                             d.getCurrentPatientCount(), d.getMaxPatients());
                    drawText(infoX + 20.0f, rowY, rowBuf, GLColor(0.15f, 0.20f, 0.28f));
                    rowY += 32.0f;
                }
            }
            drawRectangle(infoX, infoY + infoH - 45.0f, infoW, 45.0f, GLColor(0.95f, 0.97f, 1.0f), true);
            drawText(infoX + 20.0f, infoY + infoH - 18.0f, "Association: Patient assignments tracked per Doctor with strict capacity limits", GLColor(0.2f, 0.4f, 0.65f));
            break;
        }
        case 3: { // Appointments
            drawText(infoX + 20.0f, infoY + 26.0f, "APPOINTMENTS & OPD QUEUE (DS/PL CO3 FIFO QUEUE)", GLColor(1.0f, 1.0f, 1.0f));
            drawText(infoX + 20.0f, infoY + 65.0f, "APPT#   PATIENT ID   DOCTOR ID   SCHEDULED TIME      PRIORITY    STATUS", GLColor(0.35f, 0.45f, 0.55f));
            drawLine(infoX + 20.0f, infoY + 74.0f, infoX + infoW - 20.0f, infoY + 74.0f, GLColor(0.80f, 0.85f, 0.90f), 1.5f);

            float rowY = infoY + 98.0f;
            if (hospitalRef && !hospitalRef->getAllAppointments().empty()) {
                int count = 0;
                for (const auto& pair : hospitalRef->getAllAppointments()) {
                    if (count++ >= 8) break;
                    const Appointment& a = pair.second;
                    char rowBuf[128];
                    snprintf(rowBuf, sizeof(rowBuf), "#%-5d  P%-9d  D%-9d  %-16s  %-10s  %s",
                             a.getAppointmentId(), a.getPatientId(), a.getDoctorId(),
                             a.getDateTime().substr(0, 16).c_str(),
                             a.getPriorityString().c_str(),
                             a.getStatusString().c_str());
                    drawText(infoX + 20.0f, rowY, rowBuf, GLColor(0.15f, 0.20f, 0.28f));
                    rowY += 32.0f;
                }
            } else {
                drawText(infoX + 20.0f, rowY, "No scheduled appointments in queue.", GLColor(0.4f, 0.4f, 0.4f));
            }
            drawRectangle(infoX, infoY + infoH - 45.0f, infoW, 45.0f, GLColor(0.95f, 0.97f, 1.0f), true);
            drawText(infoX + 20.0f, infoY + infoH - 18.0f, "DS CO3: First-In-First-Out (FIFO) queue guarantees starvation-free token turns", GLColor(0.2f, 0.4f, 0.65f));
            break;
        }
        case 4: { // Ward Beds
            drawText(infoX + 20.0f, infoY + 26.0f, "HOSPITAL WARD & BED OCCUPANCY (DS CO1 FIXED ARRAY)", GLColor(1.0f, 1.0f, 1.0f));
            float textY = infoY + 70.0f;
            drawText(infoX + 20.0f, textY, "Ward 1: 20 General Beds [FixedArray<Bed, 20> In-Memory]", GLColor(0.15f, 0.25f, 0.35f)); textY += 26.0f;
            drawText(infoX + 20.0f, textY, "ICU Ward: 5 Critical Beds | Total Capacity: 25 Beds", GLColor(0.15f, 0.25f, 0.35f)); textY += 32.0f;

            if (hospitalRef && !hospitalRef->getAllBeds().empty()) {
                int count = 0;
                for (const auto& pair : hospitalRef->getAllBeds()) {
                    const Bed& b = pair.second;
                    if (b.getIsOccupied() || count < 6) {
                        if (count++ >= 8) break;
                        char bedBuf[128];
                        if (b.getIsOccupied()) {
                            snprintf(bedBuf, sizeof(bedBuf), "Bed #%-2d [%-11s] : OCCUPIED (Patient P%d) - Rs %.0f/day",
                                     b.getBedId(), b.getType().c_str(), b.getOccupiedByPatientId(), b.getDailyRate());
                            drawText(infoX + 25.0f, textY, bedBuf, GLColor(0.85f, 0.2f, 0.2f));
                        } else {
                            snprintf(bedBuf, sizeof(bedBuf), "Bed #%-2d [%-11s] : AVAILABLE               - Rs %.0f/day",
                                     b.getBedId(), b.getType().c_str(), b.getDailyRate());
                            drawText(infoX + 25.0f, textY, bedBuf, GLColor(0.15f, 0.6f, 0.25f));
                        }
                        textY += 28.0f;
                    }
                }
            }
            drawRectangle(infoX, infoY + infoH - 45.0f, infoW, 45.0f, GLColor(0.95f, 0.97f, 1.0f), true);
            drawText(infoX + 20.0f, infoY + infoH - 18.0f, "DS CO1: FixedArray provides O(1) index access with zero heap allocation overhead", GLColor(0.2f, 0.4f, 0.65f));
            break;
        }
        case 5: { // Pharmacy Stock
            drawText(infoX + 20.0f, infoY + 26.0f, "PHARMACY INVENTORY & FORMULARY", GLColor(1.0f, 1.0f, 1.0f));
            drawText(infoX + 20.0f, infoY + 65.0f, "CODE     NAME                CATEGORY        PRICE    STOCK   STATUS", GLColor(0.35f, 0.45f, 0.55f));
            drawLine(infoX + 20.0f, infoY + 74.0f, infoX + infoW - 20.0f, infoY + 74.0f, GLColor(0.80f, 0.85f, 0.90f), 1.5f);

            float rowY = infoY + 98.0f;
            if (hospitalRef && !hospitalRef->getAllMedicines().empty()) {
                int count = 0;
                for (const auto& pair : hospitalRef->getAllMedicines()) {
                    if (count++ >= 8) break;
                    const Medicine& m = pair.second;
                    char rowBuf[128];
                    std::string nameSub = m.getName().substr(0, 16);
                    std::string statStr = m.isLowStock() ? "LOW STOCK" : "IN STOCK";
                    snprintf(rowBuf, sizeof(rowBuf), "%-7s  %-16s  %-14s  Rs %-4.0f  %-5d   %s",
                             m.getId().c_str(), nameSub.c_str(), m.getCategory().substr(0, 14).c_str(),
                             m.getPrice(), m.getQuantity(), statStr.c_str());
                    GLColor col = m.isLowStock() ? GLColor(0.85f, 0.2f, 0.2f) : GLColor(0.15f, 0.20f, 0.28f);
                    drawText(infoX + 20.0f, rowY, rowBuf, col);
                    rowY += 32.0f;
                }
            }
            drawRectangle(infoX, infoY + infoH - 45.0f, infoW, 45.0f, GLColor(0.95f, 0.97f, 1.0f), true);
            drawText(infoX + 20.0f, infoY + infoH - 18.0f, "Encapsulation: Safe inventory dispensing with minimum reorder threshold guard", GLColor(0.2f, 0.4f, 0.65f));
            break;
        }
        case 6: { // Billing & Invoices
            drawText(infoX + 20.0f, infoY + 26.0f, "BILLING & PATIENT INVOICES", GLColor(1.0f, 1.0f, 1.0f));
            drawText(infoX + 20.0f, infoY + 65.0f, "BILL#   PATIENT   CONSULT   MEDS    ROOM     GST(18%)  TOTAL    STATUS", GLColor(0.35f, 0.45f, 0.55f));
            drawLine(infoX + 20.0f, infoY + 74.0f, infoX + infoW - 20.0f, infoY + 74.0f, GLColor(0.80f, 0.85f, 0.90f), 1.5f);

            float rowY = infoY + 98.0f;
            if (hospitalRef && !hospitalRef->getAllBills().empty()) {
                int count = 0;
                for (const auto& pair : hospitalRef->getAllBills()) {
                    if (count++ >= 8) break;
                    const Billing& b = pair.second;
                    char rowBuf[128];
                    snprintf(rowBuf, sizeof(rowBuf), "#%-5d  P%-7d  Rs %-4.0f  Rs %-3.0f  Rs %-4.0f  Rs %-4.0f  Rs %-5.0f %s",
                             b.getBillId(), b.getPatientId(), b.getConsultationCharge(),
                             b.getMedicineCharge(), b.getRoomCharge(),
                             (b.getSubtotal() * 0.18), b.getTotalAmount(),
                             b.getPaymentStatusString().c_str());
                    drawText(infoX + 20.0f, rowY, rowBuf, GLColor(0.15f, 0.20f, 0.28f));
                    rowY += 32.0f;
                }
            } else {
                drawText(infoX + 20.0f, rowY, "No invoices generated yet.", GLColor(0.4f, 0.4f, 0.4f));
            }
            drawRectangle(infoX, infoY + infoH - 45.0f, infoW, 45.0f, GLColor(0.95f, 0.97f, 1.0f), true);
            drawText(infoX + 20.0f, infoY + infoH - 18.0f, "COA Integration: Line items compiled into 32-bit registers for ALU execution", GLColor(0.2f, 0.4f, 0.65f));
            break;
        }
        case 9: { // 80386 Hardware Billing
            drawText(infoX + 20.0f, infoY + 26.0f, "80386 MICROPROCESSOR SIMULATION (COA CO1 - CO4)", GLColor(1.0f, 1.0f, 1.0f));
            float textY = infoY + 68.0f;
            drawText(infoX + 20.0f, textY, "32-Bit General-Purpose Registers:", GLColor(0.15f, 0.25f, 0.35f)); textY += 24.0f;
            drawText(infoX + 35.0f, textY, "EAX (Accumulator) : 0x00001211  (Consultation + Room = 4625 dec)", GLColor(0.10f, 0.45f, 0.25f)); textY += 24.0f;
            drawText(infoX + 35.0f, textY, "EBX (Base)        : 0x00000120  (Pharmacy + Diagnostic Tests)", GLColor(0.10f, 0.45f, 0.25f)); textY += 24.0f;
            drawText(infoX + 35.0f, textY, "ECX (Count / Tax) : 0x000002C1  (18% GST bitwise calculated = 705 dec)", GLColor(0.10f, 0.45f, 0.25f)); textY += 24.0f;
            drawText(infoX + 35.0f, textY, "EDX (Data / Total): 0x000015F2  (Grand Total Payable = 5618 dec)", GLColor(0.10f, 0.45f, 0.25f)); textY += 28.0f;

            drawText(infoX + 20.0f, textY, "Flags Register: ZF=0 (Non-Zero) | CF=0 (No Carry) | SF=0 | PF=1", GLColor(0.75f, 0.35f, 0.1f)); textY += 26.0f;
            drawText(infoX + 20.0f, textY, "ALU Engine     : Ripple-Carry Adder with Bitwise Carry Generation", GLColor(0.15f, 0.25f, 0.35f)); textY += 26.0f;
            drawText(infoX + 20.0f, textY, "Instruction Pipeline (5 Stages):", GLColor(0.15f, 0.25f, 0.35f)); textY += 24.0f;
            drawText(infoX + 35.0f, textY, "[FETCH] -> [DECODE] -> [ADDR GEN] -> [EXECUTE] -> [WRITEBACK]", GLColor(0.20f, 0.45f, 0.75f)); textY += 28.0f;
            drawText(infoX + 20.0f, textY, "Assembly: Real MASM/NASM 80386 assembly executed in backend", GLColor(0.35f, 0.45f, 0.55f));

            drawRectangle(infoX, infoY + infoH - 45.0f, infoW, 45.0f, GLColor(0.95f, 0.97f, 1.0f), true);
            drawText(infoX + 20.0f, infoY + infoH - 18.0f, "COA CO1-CO4: 32-bit registers, ripple-carry addition, pipeline stages & flags", GLColor(0.2f, 0.4f, 0.65f));
            break;
        }
        case 12: { // Data Structures Demo
            drawText(infoX + 20.0f, infoY + 26.0f, "DATA STRUCTURES ARCHITECTURE (DS/PL CO1 - CO5)", GLColor(1.0f, 1.0f, 1.0f));
            float textY = infoY + 68.0f;
            drawText(infoX + 20.0f, textY, "1. DS CO1 - FixedArray<Bed, 20>:", GLColor(0.15f, 0.25f, 0.35f)); textY += 22.0f;
            drawText(infoX + 40.0f, textY, "Fixed ward beds, zero overhead, O(1) direct slot index access", GLColor(0.35f, 0.45f, 0.55f)); textY += 26.0f;
            drawText(infoX + 20.0f, textY, "2. DS CO2 - Doubly LinkedList<MedicalRecord>:", GLColor(0.15f, 0.25f, 0.35f)); textY += 22.0f;
            drawText(infoX + 40.0f, textY, "Dynamic patient medical history chain, bidirectional traversal", GLColor(0.35f, 0.45f, 0.55f)); textY += 26.0f;
            drawText(infoX + 20.0f, textY, "3. DS CO3 - Stack<int> & Queue<int> & PriorityQueue<int>:", GLColor(0.15f, 0.25f, 0.35f)); textY += 22.0f;
            drawText(infoX + 40.0f, textY, "LIFO audit trail, FIFO appointments, Min-Heap emergency triage", GLColor(0.35f, 0.45f, 0.55f)); textY += 26.0f;
            drawText(infoX + 20.0f, textY, "4. DS CO4 - QuickSort & Binary Search:", GLColor(0.15f, 0.25f, 0.35f)); textY += 22.0f;
            drawText(infoX + 40.0f, textY, "O(n log n) divide-and-conquer sorting, O(log n) patient search", GLColor(0.35f, 0.45f, 0.55f)); textY += 26.0f;
            drawText(infoX + 20.0f, textY, "5. DS CO5 - PatientHashTable (Quadratic Probing):", GLColor(0.15f, 0.25f, 0.35f)); textY += 22.0f;
            drawText(infoX + 40.0f, textY, "O(1) average lookup time with quadratic open-address collision handling", GLColor(0.35f, 0.45f, 0.55f));

            drawRectangle(infoX, infoY + infoH - 45.0f, infoW, 45.0f, GLColor(0.95f, 0.97f, 1.0f), true);
            drawText(infoX + 20.0f, infoY + infoH - 18.0f, "DS CO1-CO5: All 5 course outcomes custom implemented without external STL DS", GLColor(0.2f, 0.4f, 0.65f));
            break;
        }
        case 13: { // System Status Overview
            drawText(infoX + 20.0f, infoY + 26.0f, "SMART HOSPITAL SYSTEM AUDIT & METRICS", GLColor(1.0f, 1.0f, 1.0f));
            float textY = infoY + 68.0f;
            size_t pTotal = hospitalRef ? hospitalRef->getTotalPatients() : 4;
            size_t dTotal = hospitalRef ? hospitalRef->getTotalDoctors() : 5;
            size_t bTotal = hospitalRef ? hospitalRef->getTotalBeds() : 25;
            int bOcc = hospitalRef ? hospitalRef->getOccupiedBedCount() : 4;
            int bFree = hospitalRef ? hospitalRef->getAvailableBedCount() : 21;
            size_t emgSize = hospitalRef ? hospitalRef->getEmergencyCount() : 1;
            size_t apptSize = hospitalRef ? hospitalRef->getPendingAppointmentsCount() : 3;

            char buf[128];
            snprintf(buf, sizeof(buf), "Total Patients Registered : %u", (unsigned)pTotal);
            drawText(infoX + 25.0f, textY, buf, GLColor(0.15f, 0.25f, 0.35f)); textY += 26.0f;
            snprintf(buf, sizeof(buf), "Active Duty Doctors       : %u", (unsigned)dTotal);
            drawText(infoX + 25.0f, textY, buf, GLColor(0.15f, 0.25f, 0.35f)); textY += 26.0f;
            snprintf(buf, sizeof(buf), "Ward Beds Occupancy       : %d / %u (Available: %d)", bOcc, (unsigned)bTotal, bFree);
            drawText(infoX + 25.0f, textY, buf, GLColor(0.15f, 0.25f, 0.35f)); textY += 26.0f;
            snprintf(buf, sizeof(buf), "Emergency Triage Cases    : %u Critical", (unsigned)emgSize);
            drawText(infoX + 25.0f, textY, buf, GLColor(0.85f, 0.25f, 0.25f)); textY += 26.0f;
            snprintf(buf, sizeof(buf), "Appointments in Queue     : %u Pending", (unsigned)apptSize);
            drawText(infoX + 25.0f, textY, buf, GLColor(0.15f, 0.25f, 0.35f)); textY += 32.0f;

            drawText(infoX + 20.0f, textY, "Academic Integration: 4 Core Subjects Unified", GLColor(0.15f, 0.25f, 0.35f)); textY += 24.0f;
            drawText(infoX + 35.0f, textY, "PSOOP (2310261L) + DS/PL (2310214L) + COA (2310221L) + CG (2310218L)", GLColor(0.35f, 0.45f, 0.55f)); textY += 26.0f;
            drawText(infoX + 20.0f, textY, "Institution: MIT Academy of Engineering, Pune", GLColor(0.15f, 0.25f, 0.35f)); textY += 24.0f;
            drawText(infoX + 35.0f, textY, "Team: Sashwat Shende, Smit Redij, Vighnesh Chityal, Vedant Thakare", GLColor(0.25f, 0.45f, 0.7f));

            drawRectangle(infoX, infoY + infoH - 45.0f, infoW, 45.0f, GLColor(0.95f, 0.97f, 1.0f), true);
            drawText(infoX + 20.0f, infoY + infoH - 18.0f, "System Status: 100% HEALTHY - ALL CORE MODULES ONLINE", GLColor(0.1f, 0.6f, 0.25f));
            break;
        }
        default: { // Overview (0)
            drawText(infoX + 20.0f, infoY + 26.0f, "SYSTEM ARCHITECTURE STATUS (16:9 HIGH-DEFINITION VIEW)", GLColor(1.0f, 1.0f, 1.0f));
            float textY = infoY + 68.0f;
            drawText(infoX + 20.0f, textY, "PSOOP Module: Domain Models & Facade Pattern", GLColor(0.15f, 0.25f, 0.35f)); textY += 26.0f;
            drawText(infoX + 35.0f, textY, "  - Person, Patient, Doctor, Bed, Medicine, Record, Billing, Hospital", GLColor(0.35f, 0.45f, 0.55f)); textY += 32.0f;
            drawText(infoX + 20.0f, textY, "DS/PL Module: High-Performance Data Structures", GLColor(0.15f, 0.25f, 0.35f)); textY += 26.0f;
            drawText(infoX + 35.0f, textY, "  - FixedArray (Beds), LinkedList (History), Stack (Audit), Queues, Hash", GLColor(0.35f, 0.45f, 0.55f)); textY += 32.0f;
            drawText(infoX + 20.0f, textY, "COA Module: 80386 Hardware Processor Simulation", GLColor(0.15f, 0.25f, 0.35f)); textY += 26.0f;
            drawText(infoX + 35.0f, textY, "  - 32-bit Registers, Ripple-Carry ALU, 5-Stage Pipelining, NASM Assembly", GLColor(0.35f, 0.45f, 0.55f)); textY += 32.0f;
            drawText(infoX + 20.0f, textY, "CG Module: OpenGL Visualization & Simulation", GLColor(0.15f, 0.25f, 0.35f)); textY += 26.0f;
            drawText(infoX + 35.0f, textY, "  - Smooth Primitives, Anti-Aliased Lines, Geometric Transformations", GLColor(0.35f, 0.45f, 0.55f)); textY += 38.0f;

            drawText(infoX + 20.0f, textY, ">>> INTERACTION GUIDE:", GLColor(0.85f, 0.45f, 0.1f)); textY += 24.0f;
            drawText(infoX + 35.0f, textY, "* Click buttons 1-6 or 9-13 to view live module panels in this card", GLColor(0.2f, 0.4f, 0.65f)); textY += 22.0f;
            drawText(infoX + 35.0f, textY, "* Click [7] for Architectural Layout, [8] for Emergency Animation", GLColor(0.2f, 0.4f, 0.65f));

            drawRectangle(infoX, infoY + infoH - 45.0f, infoW, 45.0f, GLColor(0.95f, 0.97f, 1.0f), true);
            drawText(infoX + 20.0f, infoY + infoH - 18.0f, "Ready | Click buttons on left or top stat cards to interactively navigate", GLColor(0.2f, 0.5f, 0.3f));
            break;
        }
    }

    // Bottom Footer
    drawRectangle(0.0f, windowHeight - 35.0f, windowWidth, 35.0f, GLColor(0.15f, 0.20f, 0.28f), true);
    drawText(30.0f, windowHeight - 12.0f, "Ready | Click Any Button (1-6, 9-13) or Stat Card to Switch Panels | Mode 7 Layout | Mode 8 Cart Sim", GLColor(0.85f, 0.9f, 0.95f));
}

void HospitalGraphics::renderHospitalLayout() {
    // Floor Base
    drawRectangle(0.0f, 0.0f, windowWidth, windowHeight, GLColor(0.85f, 0.86f, 0.88f), true);

    // Corridors
    drawCorridors();

    // 1. Reception & Waiting Area (Top Left: x=50, y=50, w=440, h=180)
    drawRoom(50.0f, 50.0f, 440.0f, 180.0f, "RECEPTION & OUTPATIENT WAITING", GLColor(0.96f, 0.96f, 0.98f));
    // Reception Desk
    drawRectangle(80.0f, 100.0f, 160.0f, 28.0f, GLColor(0.55f, 0.38f, 0.22f), true);
    drawText(100.0f, 118.0f, "Check-in Desk", GLColor(1.0f, 1.0f, 1.0f));
    // Waiting Chairs
    for (int i = 0; i < 6; ++i) {
        drawChair(280.0f, 90.0f + i * 22.0f);
        drawChair(330.0f, 90.0f + i * 22.0f);
        drawChair(380.0f, 90.0f + i * 22.0f);
    }

    // 2. Pharmacy (Top: x=50, y=250, w=210, h=190)
    drawRoom(50.0f, 250.0f, 210.0f, 190.0f, "PHARMACY DISPENSARY", GLColor(0.94f, 0.98f, 0.94f));
    drawRectangle(70.0f, 290.0f, 170.0f, 24.0f, GLColor(0.3f, 0.6f, 0.35f), true);
    drawText(85.0f, 307.0f, "Medicine Counter", GLColor(1.0f, 1.0f, 1.0f));
    drawRectangle(70.0f, 330.0f, 170.0f, 16.0f, GLColor(0.7f, 0.75f, 0.7f), true);
    drawRectangle(70.0f, 360.0f, 170.0f, 16.0f, GLColor(0.7f, 0.75f, 0.7f), true);
    drawRectangle(70.0f, 390.0f, 170.0f, 16.0f, GLColor(0.7f, 0.75f, 0.7f), true);

    // 3. Diagnostic Lab (Top: x=280, y=250, w=210, h=190)
    drawRoom(280.0f, 250.0f, 210.0f, 190.0f, "DIAGNOSTIC & LAB", GLColor(0.95f, 0.95f, 0.98f));
    drawRectangle(295.0f, 290.0f, 175.0f, 32.0f, GLColor(0.35f, 0.45f, 0.65f), true);
    drawText(310.0f, 312.0f, "Automated Analyzers", GLColor(1.0f, 1.0f, 1.0f));
    drawCircle(385.0f, 365.0f, 24.0f, GLColor(0.3f, 0.7f, 0.8f), 48, true); // Centrifuge

    // 4. Emergency Room (Bottom Left: x=50, y=555, w=440, h=135)
    drawRoom(50.0f, 555.0f, 440.0f, 135.0f, "EMERGENCY & TRAUMA CARE", GLColor(1.0f, 0.94f, 0.94f));
    drawMedicalCross(455.0f, 570.0f, 18.0f, GLColor(0.9f, 0.1f, 0.1f));
    // Emergency Trauma Bays
    drawBed(90.0f, 595.0f, true, 23);
    drawBed(180.0f, 595.0f, false, 24);
    drawBed(270.0f, 595.0f, false, 25);

    // 5. General Ward 1 (Bottom Right: x=510, y=555, w=720, h=135)
    drawRoom(510.0f, 555.0f, 720.0f, 135.0f, "WARD 1: GENERAL INPATIENT (DS CO1 FIXED ARRAY)", GLColor(0.95f, 0.97f, 0.95f));
    // Algorithmic Grid Bed Placement (CG CO3 Algorithm)
    for (int i = 0; i < 8; ++i) {
        float bx = 535.0f + i * 86.0f;
        bool occ = (i == 0 || i == 3 || i == 6);
        drawBed(bx, 595.0f, occ, i + 1);
    }

    // 6. Intensive Care Unit - ICU (Top Right: x=620, y=50, w=610, h=175)
    drawRoom(620.0f, 50.0f, 610.0f, 175.0f, "INTENSIVE CARE UNIT (ICU)", GLColor(0.93f, 0.96f, 1.0f));
    for (int i = 0; i < 5; ++i) {
        float bx = 650.0f + i * 110.0f;
        bool occ = (i < 2); // Beds 19, 20 occupied
        drawBed(bx, 90.0f, occ, 19 + i);
        // Vital signs monitors next to beds
        drawRectangle(bx + 56.0f, 95.0f, 18.0f, 28.0f, GLColor(0.1f, 0.1f, 0.1f), true);
        drawRectangle(bx + 58.0f, 97.0f, 14.0f, 24.0f, GLColor(0.1f, 0.85f, 0.25f), true);
    }

    // 7. Operation Theatre / Surgery (Middle Right: x=620, y=245, w=610, h=195)
    drawRoom(620.0f, 245.0f, 610.0f, 195.0f, "OPERATION THEATRE & SURGERY SUITE", GLColor(0.92f, 0.98f, 0.98f));
    // Surgical table in center
    drawRectangle(860.0f, 315.0f, 95.0f, 44.0f, GLColor(0.6f, 0.65f, 0.7f), true);
    drawCircle(907.0f, 337.0f, 28.0f, GLColor(1.0f, 1.0f, 0.8f, 0.45f), 48, true);

    // Navigation Banner at top with Back Button
    drawRectangle(50.0f, 10.0f, 210.0f, 32.0f, GLColor(0.18f, 0.42f, 0.72f), true);
    drawRectangle(50.0f, 10.0f, 210.0f, 32.0f, GLColor(1.0f, 1.0f, 1.0f), false);
    drawText(62.0f, 31.0f, "<< BACK TO DASHBOARD", GLColor(1.0f, 1.0f, 1.0f));

    drawRectangle(270.0f, 10.0f, 960.0f, 32.0f, GLColor(0.15f, 0.22f, 0.32f), true);
    drawText(290.0f, 31.0f, "HOSPITAL ARCHITECTURAL TOP-DOWN VIEW (16:9 HD) | Press 'ESC' or Click Back to return to Dashboard", GLColor(1.0f, 1.0f, 1.0f));
}

void HospitalGraphics::renderEmergencySimulation() {
    // 1. Render Hospital Layout as Base
    renderHospitalLayout();

    // 2. Highlight Emergency Path with Animated Corridor Arrows
    for (size_t i = 0; i + 1 < cart.path.size(); ++i) {
        drawLine(cart.path[i].first, cart.path[i].second,
                 cart.path[i + 1].first, cart.path[i + 1].second,
                 GLColor(0.9f, 0.3f, 0.3f, 0.7f), 4.0f);
    }

    // 3. Apply Transformations to Patient Cart (CG CO2)
    applyCartTransformations();

    // 4. Update Cart Physics/Position
    updateCartAnimation();

    // 5. On-screen Controls Overlay Banner with Back Button
    drawRectangle(50.0f, 10.0f, 210.0f, 35.0f, GLColor(0.18f, 0.42f, 0.72f), true);
    drawRectangle(50.0f, 10.0f, 210.0f, 35.0f, GLColor(1.0f, 1.0f, 1.0f), false);
    drawText(62.0f, 32.0f, "<< BACK TO DASHBOARD", GLColor(1.0f, 1.0f, 1.0f));

    drawRectangle(270.0f, 10.0f, 960.0f, 35.0f, GLColor(0.85f, 0.15f, 0.15f), true);
    drawText(285.0f, 32.0f, "CRITICAL EMERGENCY PATIENT CART SIMULATION (ER -> ICU BED #19)", GLColor(1.0f, 1.0f, 1.0f));

    // Interactive Key Instructions Box on right
    float boxX = 860.0f;
    float boxY = 385.0f;
    drawRectangle(boxX, boxY, 360.0f, 135.0f, GLColor(0.12f, 0.16f, 0.24f, 0.95f), true);
    drawRectangle(boxX, boxY, 360.0f, 135.0f, GLColor(0.9f, 0.2f, 0.2f), false);

    drawText(boxX + 15.0f, boxY + 24.0f, "TRANSFORMATION CONTROLS (CG CO2):", GLColor(1.0f, 0.85f, 0.2f));
    drawText(boxX + 15.0f, boxY + 47.0f, "[T] or [UP]    : Move Forward Along Path", GLColor(1.0f, 1.0f, 1.0f));
    drawText(boxX + 15.0f, boxY + 69.0f, "[R] or [RIGHT] : Rotate 90 deg at Corner", GLColor(1.0f, 1.0f, 1.0f));
    drawText(boxX + 15.0f, boxY + 91.0f, "[S]            : Scale (Zoom Cart 1.4x)", GLColor(1.0f, 1.0f, 1.0f));
    drawText(boxX + 15.0f, boxY + 111.0f, "[A]            : Reset to Emergency Room", GLColor(1.0f, 1.0f, 1.0f));
    drawText(boxX + 15.0f, boxY + 128.0f, "[ESC]          : Return to Dashboard", GLColor(0.7f, 0.85f, 1.0f));
}

void HospitalGraphics::render() {
    glClearColor(0.94f, 0.95f, 0.97f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    setupOrthographicProjection();

    switch (currentMode) {
        case DASHBOARD_MODE:
            renderDashboardGUI();
            break;
        case HOSPITAL_VIEW_MODE:
            renderHospitalLayout();
            break;
        case EMERGENCY_SIM_MODE:
            renderEmergencySimulation();
            break;
    }

    glFlush();
}

void HospitalGraphics::triggerEmergencySimulation() {
    currentMode = EMERGENCY_SIM_MODE;
    cart.reset();
    cart.isMoving = true;
}

void HospitalGraphics::handleKeyPress(char key) {
    switch (key) {
        case 't': case 'T':
            if (currentMode == EMERGENCY_SIM_MODE) {
                cart.isMoving = true;
            }
            break;
        case 'r': case 'R':
            if (currentMode == EMERGENCY_SIM_MODE) {
                cart.rotate(90.0f);
            }
            break;
        case 's': case 'S':
            if (currentMode == EMERGENCY_SIM_MODE) {
                cart.toggleScale();
            }
            break;
        case 'a': case 'A':
            if (currentMode == EMERGENCY_SIM_MODE) {
                cart.reset();
            }
            break;
        case 27: // ESC
            currentMode = DASHBOARD_MODE;
            break;
        case '1':
            currentMode = HOSPITAL_VIEW_MODE;
            break;
        case '2':
            triggerEmergencySimulation();
            break;
        case '0':
            currentMode = DASHBOARD_MODE;
            break;
        default:
            break;
    }
}

void HospitalGraphics::handleSpecialKey(int key) {
    if (currentMode == EMERGENCY_SIM_MODE) {
        // UP arrow
        if (key == 101 || key == 38) {
            cart.isMoving = true;
        }
        // RIGHT arrow
        else if (key == 102 || key == 39) {
            cart.rotate(90.0f);
        }
        // LEFT arrow
        else if (key == 100 || key == 37) {
            cart.rotate(-90.0f);
        }
    }
}

void HospitalGraphics::handleMouseMove(int x, int y) {
    if (currentMode == DASHBOARD_MODE) {
        for (auto& btn : buttons) {
            btn.isHovered = (x >= btn.x && x <= btn.x + btn.width &&
                             y >= btn.y && y <= btn.y + btn.height);
        }
    }
}

void HospitalGraphics::handleMouseClick(int button, int state, int x, int y) {
    if (button == 0 && state == 0) { // Left button press
        if (currentMode == DASHBOARD_MODE) {
            // Check top stat cards (y: 95 to 150)
            if (y >= 95 && y <= 150) {
                if (x >= 60 && x <= 330) {
                    activePanelId = 1; // Patient Records
                    return;
                } else if (x >= 350 && x <= 620) {
                    activePanelId = 2; // Doctor Roster
                    return;
                } else if (x >= 640 && x <= 910) {
                    activePanelId = 4; // Ward Beds
                    return;
                } else if (x >= 930 && x <= 1200) {
                    triggerEmergencySimulation(); // Critical Emergency Simulation
                    return;
                }
            }

            for (const auto& btn : buttons) {
                if (x >= btn.x && x <= btn.x + btn.width &&
                    y >= btn.y && y <= btn.y + btn.height) {
                    if (btn.actionId == 10) {
                        currentMode = HOSPITAL_VIEW_MODE;
                    } else if (btn.actionId == 11) {
                        triggerEmergencySimulation();
                    } else if (btn.actionId == 0) {
                        activePanelId = 0;
                        currentMode = DASHBOARD_MODE;
                    } else {
                        std::cout << "\n[GUI Action] Displaying panel for: " << btn.label << "\n";
                        activePanelId = btn.actionId;
                    }
                    return;
                }
            }
        } else {
            // In Hospital View Mode or Emergency Sim Mode:
            // Check "<< BACK TO DASHBOARD" button or top area
            if ((x >= 50 && x <= 270 && y >= 8 && y <= 45) || y < 45) {
                currentMode = DASHBOARD_MODE;
            }
        }
    }
}

// ============================================================================
// Native Windows Win32 + OpenGL Window Thread Implementation
// ============================================================================
#ifdef _WIN32
static HWND g_hWnd = nullptr;
static HDC g_hDC = nullptr;
static HGLRC g_hRC = nullptr;
static HANDLE g_hThread = nullptr;
static DWORD g_graphicsThreadId = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HospitalGraphics* gfx = HospitalGraphics::getInstance();
    switch (message) {
        case WM_ERASEBKGND:
            return 1; // Prevent GDI erasing background to eliminate flicker

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            if (g_hRC && gfx) {
                gfx->render();
                SwapBuffers(g_hDC ? g_hDC : hdc);
            }
            EndPaint(hWnd, &ps);
            return 0;
        }
        case WM_MOUSEMOVE: {
            int mx = LOWORD(lParam);
            int my = HIWORD(lParam);
            if (gfx) gfx->handleMouseMove(mx, my);
            return 0;
        }
        case WM_LBUTTONDOWN: {
            int mx = LOWORD(lParam);
            int my = HIWORD(lParam);
            if (gfx) gfx->handleMouseClick(0, 0, mx, my);
            return 0;
        }
        case WM_KEYDOWN: {
            if (gfx) {
                gfx->handleKeyPress(static_cast<char>(wParam));
                gfx->handleSpecialKey(static_cast<int>(wParam));
            }
            return 0;
        }
        case WM_CLOSE: {
            if (gfx) gfx->closeWindow();
            DestroyWindow(hWnd);
            return 0;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
        default:
            return DefWindowProcA(hWnd, message, wParam, lParam);
    }
}

void graphicsThreadFunc(void* param) {
    HospitalGraphics* gfx = static_cast<HospitalGraphics*>(param);
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    WNDCLASSEXA wc = {0};
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "SmartHospitalGLClass";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = nullptr;
    RegisterClassExA(&wc);

    RECT rect = {0, 0, gfx ? gfx->getWindowWidth() : 1280, gfx ? gfx->getWindowHeight() : 720};
    AdjustWindowRectEx(&rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE, WS_EX_APPWINDOW);
    int winW = rect.right - rect.left;
    int winH = rect.bottom - rect.top;

    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);
    int posX = (screenW - winW) / 2;
    int posY = (screenH - winH) / 2;
    if (posX < 0) posX = 20;
    if (posY < 0) posY = 20;

    // Create window WITHOUT WS_VISIBLE first so no WM_PAINT is dispatched before OpenGL is ready
    HWND hWnd = CreateWindowExA(
        WS_EX_APPWINDOW, "SmartHospitalGLClass",
        "Smart Hospital Management System - OpenGL Graphics Dashboard (16:9 HD)",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        posX, posY, winW, winH,
        nullptr, nullptr, hInstance, nullptr
    );

    if (!hWnd) {
        std::cerr << "[OpenGL Error] Window creation failed! Error code: " << GetLastError() << "\n";
        return;
    }

    g_hWnd = hWnd;
    g_hDC = GetDC(hWnd);
    if (!g_hDC) {
        std::cerr << "[OpenGL Error] GetDC failed! Error code: " << GetLastError() << "\n";
        DestroyWindow(hWnd);
        return;
    }

    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;

    int format = ChoosePixelFormat(g_hDC, &pfd);
    if (format == 0) {
        std::cerr << "[OpenGL Error] ChoosePixelFormat failed!\n";
        ReleaseDC(hWnd, g_hDC);
        DestroyWindow(hWnd);
        return;
    }

    if (!SetPixelFormat(g_hDC, format, &pfd)) {
        std::cerr << "[OpenGL Error] SetPixelFormat failed!\n";
        ReleaseDC(hWnd, g_hDC);
        DestroyWindow(hWnd);
        return;
    }

    g_hRC = wglCreateContext(g_hDC);
    if (!g_hRC) {
        std::cerr << "[OpenGL Error] wglCreateContext failed! Error code: " << GetLastError() << "\n";
        ReleaseDC(hWnd, g_hDC);
        DestroyWindow(hWnd);
        return;
    }

    if (!wglMakeCurrent(g_hDC, g_hRC)) {
        std::cerr << "[OpenGL Error] wglMakeCurrent failed! Error code: " << GetLastError() << "\n";
        wglDeleteContext(g_hRC);
        ReleaseDC(hWnd, g_hDC);
        DestroyWindow(hWnd);
        return;
    }

    // Initialize Win32 Font after wglMakeCurrent is active
    initWin32Font(g_hDC);

    // Initial render before showing to guarantee first frame is immediately ready
    if (gfx) {
        gfx->render();
        SwapBuffers(g_hDC);
    }

    // Now safely show and update window
    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);
    SetForegroundWindow(hWnd);

    MSG msg;
    while (gfx && gfx->getIsRunning()) {
        while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                goto end_loop;
            }
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }

        if (g_hRC && gfx) {
            gfx->render();
            SwapBuffers(g_hDC);
        }
        Utils::sleepMs(16); // ~60 FPS
    }

end_loop:
    wglMakeCurrent(nullptr, nullptr);
    if (g_hRC) {
        wglDeleteContext(g_hRC);
        g_hRC = nullptr;
    }
    if (g_hDC && hWnd) {
        ReleaseDC(hWnd, g_hDC);
        g_hDC = nullptr;
    }
    if (hWnd && IsWindow(hWnd)) {
        DestroyWindow(hWnd);
    }
    g_hWnd = nullptr;
}
#endif

void HospitalGraphics::runWindow(int width, int height) {
    if (isRunning) return;
    windowWidth = width;
    windowHeight = height;
    isRunning = true;
#ifdef _WIN32
    g_hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)graphicsThreadFunc, this, 0, &g_graphicsThreadId);
#endif
}

void HospitalGraphics::closeWindow() {
    if (!isRunning) return;
    isRunning = false;
#ifdef _WIN32
    if (g_hWnd && IsWindow(g_hWnd)) {
        PostMessageA(g_hWnd, WM_CLOSE, 0, 0);
    }
    if (g_hThread) {
        if (GetCurrentThreadId() != g_graphicsThreadId) {
            WaitForSingleObject(g_hThread, 1500);
        }
        CloseHandle(g_hThread);
        g_hThread = nullptr;
        g_graphicsThreadId = 0;
    }
#endif
}

// ============================================================================
// CG Course Outcome Explanations (For Viva / Evaluation)
// ============================================================================
void HospitalGraphics::explainCG_CO1_Primitives() {
    Utils::printSubHeader("CG CO1: OPENGL GRAPHICS PRIMITIVES");
    std::cout << "Primitives applied in Smart Hospital System:\n"
              << "  1. GL_QUADS: Used for room floors, walls, hospital bed frames, GUI buttons, and cart body\n"
              << "  2. GL_LINE_LOOP: Used for bed outlines, room borders, doorway openings\n"
              << "  3. GL_TRIANGLE_FAN: Used for parametric circular wheels, vital monitors, plant leaves, medical crosses\n"
              << "  4. GL_LINES: Used for corridor navigation pathways, divider lines, ECG waveforms\n"
              << "  5. Raster/Bitmap Fonts: Text labels for departments, bed numbers, and patient counts\n";
}

void HospitalGraphics::explainCG_CO2_Transformations() {
    Utils::printSubHeader("CG CO2: GEOMETRIC TRANSFORMATIONS IN REAL-TIME SIMULATION");
    std::cout << "Transformations applied to the Emergency Patient Cart:\n"
              << "  1. TRANSLATION (glTranslatef): Moves cart smoothly along the corridor path from ER to ICU\n"
              << "  2. ROTATION (glRotatef): Turns cart 90 degrees at corridor junctions to face travel direction\n"
              << "  3. SCALING (glScalef): Dynamically zooms cart size by 1.4x for high-detail examination\n"
              << "  4. MATRIX STACK (glPushMatrix / glPopMatrix): Isolates local cart transformations so the rest\n"
              << "     of the hospital floor plan remains static and unaffected\n";
}

void HospitalGraphics::explainCG_CO3_ObjectConstruction() {
    Utils::printSubHeader("CG CO3: 2D/3D OBJECT CONSTRUCTION ALGORITHMS");
    std::cout << "Object Construction Algorithms:\n"
              << "  1. Bed Construction: Outer Quad Frame + Inner Mattress Quad + Pillow Quad + Patient Head Circle\n"
              << "  2. Patient Cart: Chassis Quad + 4 Parametric Circle Wheels + Stretcher Quad + Red Cross Symbol\n"
              << "  3. Algorithmic Grid Placement: Procedural loop generating Ward 1 beds with uniform offsets:\n"
              << "     for(int i=0; i<7; i++) { drawBed(startX + i * 68, startY, occupied, i+1); }\n"
              << "  4. Parametric Circle Generation: x = cx + cos(theta)*r, y = cy + sin(theta)*r\n";
}

void HospitalGraphics::explainCG_CO4_ProjectionViewing() {
    Utils::printSubHeader("CG CO4: PROJECTION & VIEWING PIPELINE");
    std::cout << "Orthographic Projection Pipeline (gluOrtho2D):\n"
              << "  1. Model Coordinates -> Object local frame (e.g., Cart centered at (0,0))\n"
              << "  2. World Coordinates -> Hospital layout floor space (1000 x 700 pixels)\n"
              << "  3. Viewing Transform -> Identity (Camera directly overhead pointing down)\n"
              << "  4. Projection Transform -> gluOrtho2D(0, width, height, 0) top-left origin\n"
              << "  5. Viewport Transform -> glViewport maps normalized coordinates to screen window\n"
              << "Why Orthographic? Eliminates perspective distortion; parallel corridor lines remain parallel,\n"
              << "preserving exact dimensional ratios crucial for architectural hospital planning.\n";
}
