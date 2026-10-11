#ifndef HOSPITAL_GRAPHICS_H
#define HOSPITAL_GRAPHICS_H

#include <string>
#include <vector>
#include "models/Hospital.h"

// Color structure for OpenGL drawing
struct GLColor {
    float r, g, b, a;
    GLColor(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f)
        : r(r), g(g), b(b), a(a) {}
};

// UI Button representation for Dashboard GUI
struct GUIButton {
    float x, y, width, height;
    std::string label;
    int actionId;
    bool isHovered;
};

// Emergency Patient Cart with 2D Transformations (CG CO2)
struct EmergencyCart {
    float x, y;
    float angle; // Degrees: 0, 90, 180, 270
    float scale; // Scale factor (e.g. 1.0 or 1.5)
    float targetX, targetY;
    bool isMoving;
    size_t currentWayPoint;
    std::vector<std::pair<float, float>> path;

    EmergencyCart()
        : x(120.0f), y(480.0f), angle(0.0f), scale(1.0f),
          targetX(120.0f), targetY(480.0f), isMoving(false), currentWayPoint(0) {}

    void reset() {
        x = 120.0f;
        y = 480.0f;
        angle = 0.0f;
        scale = 1.0f;
        currentWayPoint = 0;
        isMoving = false;
    }

    void rotate(float deg) {
        angle += deg;
        if (angle >= 360.0f) angle -= 360.0f;
        if (angle < 0.0f) angle += 360.0f;
    }

    void toggleScale() {
        scale = (scale == 1.0f) ? 1.4f : 1.0f;
    }
};

// ============================================================================
// CG CO1, CO2, CO3, CO4: Hospital Graphics & OpenGL Visualization Engine
// Supports 3 Modes:
//   Mode 0: Interactive Dashboard GUI
//   Mode 1: Architectural Top-Down Hospital View
//   Mode 2: Real-time Emergency Patient Cart Simulation
// ============================================================================
class HospitalGraphics {
public:
    enum ViewMode { DASHBOARD_MODE = 0, HOSPITAL_VIEW_MODE = 1, EMERGENCY_SIM_MODE = 2 };

private:
    static HospitalGraphics* instance;
    Hospital* hospitalRef;

    int windowWidth;
    int windowHeight;
    ViewMode currentMode;
    std::string windowTitle;

    EmergencyCart cart;
    std::vector<GUIButton> buttons;
    bool isRunning;
    int activePanelId;

    // Drawing Primitives (CG CO1)
    void drawRectangle(float x, float y, float w, float h, const GLColor& color, bool filled = true);
    void drawCircle(float cx, float cy, float radius, const GLColor& color, int segments = 64, bool filled = true);
    void drawLine(float x1, float y1, float x2, float y2, const GLColor& color, float lineWidth = 2.0f);
    void drawText(float x, float y, const char* text, const GLColor& color);
    void drawText(float x, float y, const std::string& text, const GLColor& color) {
        drawText(x, y, text.c_str(), color);
    }

    // 2D Object Construction Algorithms (CG CO3)
    void drawBed(float x, float y, bool isOccupied, int bedNumber);
    void drawChair(float x, float y);
    void drawPatientCart();
    void drawMedicalCross(float x, float y, float size, const GLColor& color);
    void drawRoom(float x, float y, float w, float h, const std::string& title, const GLColor& floorColor);
    void drawCorridors();

    // Mode Renderers
    void renderDashboardGUI();
    void renderHospitalLayout();
    void renderEmergencySimulation();

    // Projection & Transformations (CG CO2, CG CO4)
    void setupOrthographicProjection();
    void applyCartTransformations();
    void updateCartAnimation();

    // Internal window initialization
    void initButtons();
    void setupPathWaypoints();

    HospitalGraphics();

public:
    static HospitalGraphics* getInstance();

    void setHospitalReference(Hospital* hosp) { hospitalRef = hosp; }
    void setMode(ViewMode mode) { currentMode = mode; }
    ViewMode getMode() const { return currentMode; }
    int getWindowWidth() const { return windowWidth; }
    int getWindowHeight() const { return windowHeight; }
    void setActivePanel(int id) { activePanelId = id; }
    int getActivePanel() const { return activePanelId; }

    // Control triggers
    void triggerEmergencySimulation();
    void handleKeyPress(char key);
    void handleSpecialKey(int key);
    void handleMouseClick(int button, int state, int x, int y);
    void handleMouseMove(int x, int y);

    // Render loop entry
    void render();

    // Window management (Cross-platform Win32 + GLUT abstraction)
    void runWindow(int width = 1280, int height = 720);
    void closeWindow();
    bool getIsRunning() const { return isRunning; }

    // CO Viva Explanations
    static void explainCG_CO1_Primitives();
    static void explainCG_CO2_Transformations();
    static void explainCG_CO3_ObjectConstruction();
    static void explainCG_CO4_ProjectionViewing();
};

#endif // HOSPITAL_GRAPHICS_H
