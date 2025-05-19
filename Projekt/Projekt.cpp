#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define LICZBA_OB_TEXTUR 2
unsigned int obiektyTextur[LICZBA_OB_TEXTUR];
const char* plikiTextur[LICZBA_OB_TEXTUR] = { "./textures/metal.png", "./textures/interfejs.png" };

// Stałe projekcji
enum {
    FULL_WINDOW,
    ASPECT_1_1,
    ORTO,
    FRUST
};

GLint skala = ASPECT_1_1;
GLint rzut = ORTO;
GLfloat zakres = 10.0f;
GLfloat blisko = 1.0f;
GLfloat daleko = 30.0f;
int zz = 0;

// Zmienne widoczności
int iCull = 0;
int iOutline = 0;
int iDepth = 1;
int iClock = 0;
int iFill = 0;

bool oswietlenieWlaczone = true;
bool swiatlo0Wlaczone = true;
bool swiatlo1Wlaczone = true;
float swiatlo0Zmiana = 0.0;
float swiatlo1Zmiana = 1.0;
float swiatlo0Intensywnosc = 1.0f;
float swiatlo1Intensywnosc = 0.5f;

float pozycjaReflekotraX = 3.0f;
float pozycjaReflekotraY = 3.0f;
float pozycjaReflekotraZ = 3.0f;
float kierunekReflekotraX = -1.0f;
float kierunekReflekotraY = -1.0f;
float kierunekReflekotraZ = -1.0f;
float skupienieReflektora = 25.0f;

GLdouble rotacjaX = 0;
GLdouble rotacjaY = 0;
GLdouble rotacjaZ = 0;

float kameraX = 0.0f;
float kameraY = 0.0f;
float kameraZ = 15.0f;
float predkoscKamery = 0.5f;
float kameraYaw = 0.0f;
float kameraPitch = 0.0f;
float kameraRoll = 0.0f;
float predkoscRotacjiKamery = 2.0f;

// Zmienne animacji
bool animacjaWlaczona = false;
float katDrzwi = 0.0f;
float kierunekDrzwi = 1.0f;
float predkoscDrzwi = 2.0f;

// Stałe oświetlenia
const GLfloat swiatlo_otoczenia[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat swiatlo_rozproszenia[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat swiatlo_odbicia[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat pozycja_swiatla[] = { 2.0f, 5.0f, 5.0f, 0.0f };
const GLfloat mat_otoczenia[] = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_rozproszenia[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_odbicia[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat wysoki_polysk[] = { 100.0f };


GLuint wczytajTeksture(const char* nazwaPliku) {
    GLuint idTekstury;
    glGenTextures(1, &idTekstury);

    GLint iWidth, iHeight, nrChannels;

    unsigned char* data = stbi_load(nazwaPliku, &iWidth, &iHeight, &nrChannels, 0);
    if (!data) {
        std::cerr << "Nie udalo sie zaladowac tekstury: " << nazwaPliku << std::endl;
        return 0;
    }
    std::cout << iWidth << "  " << iHeight << "\t" << nrChannels << std::endl;

    GLenum format = GL_RGB;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, idTekstury);
    glTexImage2D(GL_TEXTURE_2D, 0, format, iWidth, iHeight, 0, format, GL_UNSIGNED_BYTE, data);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //stbi_set_flip_vertically_on_load(true);



    stbi_image_free(data);

    return idTekstury;
}


void inicjalizujTekstury() {
    glGenTextures(LICZBA_OB_TEXTUR, obiektyTextur);

    for (int i = 0; i < LICZBA_OB_TEXTUR; i++) {
        obiektyTextur[i] = wczytajTeksture(plikiTextur[i]);
    }
}

void key_callback(GLFWwindow* window, int klawisz, int scancode, int akcja, int mody) {
    if (akcja == GLFW_PRESS) {
        switch (klawisz) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
            // Reset 
        case GLFW_KEY_SPACE:
            glLoadIdentity();
            break;

            // Kontrola ruchu: góra, dół, lewo, prawo
        case GLFW_KEY_D:
            glTranslated(0.2, 0, 0);
            break;
        case GLFW_KEY_A:
            glTranslated(-0.2, 0, 0);
            break;
        case GLFW_KEY_W:
            glTranslated(0, 0.2, 0);
            break;
        case GLFW_KEY_S:
            glTranslated(0, -0.2, 0);
            break;
        case GLFW_KEY_Q:
            glTranslated(0, 0, 1);
            break;
        case GLFW_KEY_E:
            glTranslated(0, 0, -1);
            break;

            // Kontrola rotacji
        case GLFW_KEY_F:
            rotacjaX += 10;
            break;
        case GLFW_KEY_G:
            rotacjaX -= 10;
            break;
        case GLFW_KEY_H:
            rotacjaY += 10;
            break;
        case GLFW_KEY_J:
            rotacjaY -= 10;
            break;
        case GLFW_KEY_K:
            rotacjaZ += 10;
            break;

            // Kontrola światła
        case GLFW_KEY_Z:
            if (!oswietlenieWlaczone) {
                glEnable(GL_LIGHTING);
                oswietlenieWlaczone = true;
            }
            else {
                glDisable(GL_LIGHTING);
                oswietlenieWlaczone = false;
            }
            break;

            // Kontrola światła 0
        case GLFW_KEY_X:
            if (!swiatlo0Wlaczone) {
                glEnable(GL_LIGHT0);
                swiatlo0Wlaczone = true;
            }
            else {
                glDisable(GL_LIGHT0);
                swiatlo0Wlaczone = false;
            }
            break;
        case GLFW_KEY_C: // zwiększenie intensywności
            swiatlo0Intensywnosc += 0.1f;
            if (swiatlo0Intensywnosc > 1.0f) { swiatlo0Intensywnosc = 1.0f; }
            break;
        case GLFW_KEY_V: // zmniejszenie intensywności
            swiatlo0Intensywnosc -= 0.1f;
            if (swiatlo0Intensywnosc < 0.0f) { swiatlo0Intensywnosc = 0.0f; }
            break;

            // Kontrola światła 1
        case GLFW_KEY_B:
            swiatlo1Wlaczone = !swiatlo1Wlaczone; 
            if (swiatlo1Wlaczone) {
                glEnable(GL_LIGHT1);
            }
            else {
                glDisable(GL_LIGHT1);
            }
            break;
        case GLFW_KEY_N: // zwiększenie intensywności
            swiatlo1Intensywnosc += 0.2f;
            if (swiatlo1Intensywnosc > 1.0f) {
                swiatlo1Intensywnosc = 1.0f;
            }
            break;
        case GLFW_KEY_M: // zmniejszenie intensywności
            swiatlo1Intensywnosc -= 0.2f;
            if (swiatlo1Intensywnosc < 0.0f) {
                swiatlo1Intensywnosc = 0.0f;
            }
            break;

            // Dodatkowa kontrola reflektora
        case GLFW_KEY_U:
            // Przesuń reflektor w górę
            pozycjaReflekotraY += 1.0f;
            break;
        case GLFW_KEY_I:
            // Przesuń reflektor do przodu
            pozycjaReflekotraZ -= 1.0f;
            break;
        case GLFW_KEY_Y:
            // Zwężenie wiązki reflektora
            skupienieReflektora -= 5.0f;
            if (skupienieReflektora < 5.0f) skupienieReflektora = 5.0f;
            break;

            // Kontrola projekcji
        case GLFW_KEY_P:
            if (rzut == ORTO) {
                rzut = FRUST;
                zz = -4;
            }
            else {
                rzut = ORTO;
                zz = 0;
            }
            break;

            // Kontrola skalowania okna
        case GLFW_KEY_O:
            if (skala == FULL_WINDOW) {
                skala = ASPECT_1_1;
            }
            else {
                skala = FULL_WINDOW;
            }
            break;

            // Kontrola widoczności
        case GLFW_KEY_1:
            iCull = ~iCull;
            break;

        case GLFW_KEY_2:
            iOutline = ~iOutline;
            break;

        case GLFW_KEY_3:
            iDepth = ~iDepth;
            break;

        case GLFW_KEY_4:
            iClock = ~iClock;
            break;

        case GLFW_KEY_5:
            iFill = ~iFill;
            break;

            // Animacja drzwi
        case GLFW_KEY_T:
            animacjaWlaczona = !animacjaWlaczona;
            if (!animacjaWlaczona) {
                katDrzwi = 0.0f;
                kierunekDrzwi = 1.0f;
            }
            break;
        }
    }
}


void rysujPodloge() {
    glPushMatrix();

    glTranslatef(0.0f, -4.0f, 0.0f);
    glScalef(7.0f, 0.2f, 7.0f);

    glColor3f(0.7f, 0.7f, 0.7f);

    // Górna ściana
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glEnd();

    // Dolna ściana
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();

    // Przednia ściana
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glEnd();

    // Tylna ściana
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();

    // Lewa ściana
    glBegin(GL_QUADS);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glEnd();

    // Prawa ściana
    glBegin(GL_QUADS);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glEnd();

    glPopMatrix();
}

void rysujKuchenke() {
    glPushMatrix();

    glColor3f(1.0f, 1.0f, 1.0f); // Biały

    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glScalef(4.0f, 2.5f, 2.5f);

    glBindTexture(GL_TEXTURE_2D, obiektyTextur[0]); // metal

    // Przednia ściana
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glEnd();

    // Tylna ściana
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glEnd();

    // Lewa ściana
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glEnd();

    // Prawa ściana
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glEnd();

    // Górna ściana
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glEnd();

    // Dolna ściana
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();

    glPopMatrix();

    // drzwi
    glPushMatrix();
    glTranslatef(-4.0f, -1.0f, 2.6f); 
    glRotatef(-katDrzwi, 0.0f, 1.0f, 0.0f);

    glColor3f(0.3f, 0.3f, 0.3f); // Ciemnoszary

    // Przód ramy drzwi
    glBegin(GL_QUADS);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(5.6f, 0.0f, 0.0f);
    glVertex3f(5.6f, 2.5f, 0.0f);
    glVertex3f(0.0f, 2.5f, 0.0f);  
    glEnd();

    // Tył ramy drzwi
    glBegin(GL_QUADS);
    glVertex3f(0.0f, 0.0f, -0.1f);
    glVertex3f(5.6f, 0.0f, -0.1f);
    glVertex3f(5.6f, 2.5f, -0.1f);
    glVertex3f(0.0f, 2.5f, -0.1f);
    glEnd();

    // Górna część drzwi
    glBegin(GL_QUADS);
    glVertex3f(0.0f, 2.5f, 0.0f);
    glVertex3f(5.6f, 2.5f, 0.0f);
    glVertex3f(5.6f, 2.5f, -0.1f);
    glVertex3f(0.0f, 2.5f, -0.1f);
    glEnd();

    // Dolna część drzwi
    glBegin(GL_QUADS);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(5.6f, 0.0f, 0.0f);
    glVertex3f(5.6f, 0.0f, -0.1f);
    glVertex3f(0.0f, 0.0f, -0.1f);
    glEnd();

    // Lewa strona drzwi
    glBegin(GL_QUADS);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 2.5f, 0.0f);
    glVertex3f(0.0f, 2.5f, -0.1f);
    glVertex3f(0.0f, 0.0f, -0.1f);
    glEnd();

    // Prawa strona drzwi
    glBegin(GL_QUADS);
    glVertex3f(5.6f, 0.0f, 0.0f);
    glVertex3f(5.6f, 2.5f, 0.0f);
    glVertex3f(5.6f, 2.5f, -0.1f);
    glVertex3f(5.6f, 0.0f, -0.1f);
    glEnd();

    glPopMatrix();

    glPopMatrix();

    // Panel sterowania
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();

    glTranslatef(2.0f, 0.0f, 2.6f);
    glScalef(1.8f, 3.5f, 0.01f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, obiektyTextur[1]); // Interfejs

    glBegin(GL_QUADS);
    glTexCoord2f(0.9f, 1.0f); glVertex3f(0.0f, -0.5f, 0.0f); 
    glTexCoord2f(0.1f, 1.0f); glVertex3f(1.0f, -0.5f, 0.0f);
    glTexCoord2f(0.1f, 0.0f); glVertex3f(1.0f, 0.5f, 0.0f);
    glTexCoord2f(0.9f, 0.0f); glVertex3f(0.0f, 0.5f, 0.0f);
    glEnd();

    glPopMatrix();
}

void display(float size) {
    const double a = glfwGetTime();

    glEnable(GL_TEXTURE_2D);

    if (iCull)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);

    if (iDepth)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    if (iClock)
        glFrontFace(GL_CW);
    else
        glFrontFace(GL_CCW);

    if (iFill)
        glShadeModel(GL_FLAT);
    else
        glShadeModel(GL_SMOOTH);

    glPushMatrix();

    glTranslated(0, 0, zz);

    ////////////////////////////////////////////////////////OBROT
    glRotatef(rotacjaX, 1, 0, 0);
    glRotatef(rotacjaY, 0, 1, 0);
    glRotatef(rotacjaZ, 0, 0, 1);

    //SWIATLO       0   Glowne
    GLfloat fPozycja_swiatla[4] = { swiatlo0Zmiana, 10.0f, 5.0f, 1.0f };
    GLfloat fSlabe_swiatlo[] = { 0.25f, 0.25f, 0.25f, 1.0f };
    GLfloat fMocne_swiatlo[] = { swiatlo0Intensywnosc, swiatlo0Intensywnosc, swiatlo0Intensywnosc, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, fSlabe_swiatlo); // światło otoczenia 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, fMocne_swiatlo); // światło rozproszenia 
    glLightfv(GL_LIGHT0, GL_SPECULAR, fMocne_swiatlo); // światło odbite 
    glLightfv(GL_LIGHT0, GL_POSITION, fPozycja_swiatla); // pozycja światła   

    //SWIATLO       1   Reflektor
    GLfloat fPozycja_reflektora[4] = { pozycjaReflekotraX, pozycjaReflekotraY, pozycjaReflekotraZ, 1.0f };
    GLfloat fKierunek_reflektora[4] = { kierunekReflekotraX, kierunekReflekotraY, kierunekReflekotraZ, 0.0f };
    GLfloat fSlabe_swiatlo1[] = { 0.1f * swiatlo1Intensywnosc, 0.1f * swiatlo1Intensywnosc, 0.1f * swiatlo1Intensywnosc, 1.0f };
    GLfloat fMocne_swiatlo1[] = { swiatlo1Intensywnosc, swiatlo1Intensywnosc * 0.7f, swiatlo1Intensywnosc * 0.7f, 1.0f };

    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, skupienieReflektora);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 10.0f);
    glLightfv(GL_LIGHT1, GL_AMBIENT, fSlabe_swiatlo1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, fMocne_swiatlo1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, fMocne_swiatlo1);
    glLightfv(GL_LIGHT1, GL_POSITION, fPozycja_reflektora);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, fKierunek_reflektora);

    if (swiatlo1Wlaczone) {
        glEnable(GL_LIGHT1);
    }
    else {
        glDisable(GL_LIGHT1);
    }

    glEnable(GL_TEXTURE_2D);

    rysujPodloge();

    rysujKuchenke();

    // Animacja drzwi
    if (animacjaWlaczona) {
        katDrzwi += predkoscDrzwi * kierunekDrzwi;

        if (katDrzwi >= 90.0f) {
            katDrzwi = 90.0f;
            kierunekDrzwi = -1.0f;
        }
        else if (katDrzwi <= 0.0f) {
            katDrzwi = 0.0f;
            kierunekDrzwi = 1.0f;
            animacjaWlaczona = false;
        }
    }

    glPopMatrix();
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(800, 600, "Kuchenka Mikrofalowa 3D z Teksturami", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    inicjalizujTekstury();

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        if (rzut == ORTO)
        {
            if (skala == ASPECT_1_1)
            {
                if (width < height && width > 0)
                    glOrtho(-zakres, zakres, -zakres * height / width, zakres * height / width, -zakres, zakres);
                else
                    if (width >= height && height > 0)
                        glOrtho(-zakres * width / height, zakres * width / height, -zakres, zakres, -zakres, zakres);
            }
            else
                glOrtho(-zakres, zakres, -zakres, zakres, -zakres, zakres);
        }
        if (rzut == FRUST)
        {
            if (skala == ASPECT_1_1)
            {
                if (width < height && width > 0)
                    glFrustum(-zakres, zakres, -zakres * height / width, zakres * height / width, blisko, daleko);
                else
                    if (width >= height && height > 0)
                        glFrustum(-zakres * width / height, zakres * width / height, -zakres, zakres, blisko, daleko);
            }
            else
                glFrustum(-zakres, zakres, -zakres, zakres, blisko, daleko);
        }

        glMatrixMode(GL_MODELVIEW);

        float squareSize = static_cast<float>(std::min(width, height)) * 0.2f;

        glEnable(GL_NORMALIZE);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_TEXTURE_2D);

        glLightfv(GL_LIGHT0, GL_AMBIENT, swiatlo_otoczenia);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, swiatlo_rozproszenia);
        glLightfv(GL_LIGHT0, GL_SPECULAR, swiatlo_odbicia);
        glLightfv(GL_LIGHT0, GL_POSITION, pozycja_swiatla);

        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_otoczenia);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_rozproszenia);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_odbicia);
        glMaterialfv(GL_FRONT, GL_SHININESS, wysoki_polysk);

        display(squareSize);

        glClearColor(0.6f, 0.8f, 1.0f, 1.0f); // Jasnoniebieskie tło

        // Upewnij się, że tekstura jest włączona przed zamianą buforów
        glEnable(GL_TEXTURE_2D);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteTextures(LICZBA_OB_TEXTUR, obiektyTextur);

    glfwTerminate();
    return 0;
}
