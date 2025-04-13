#include <GL/glut.h>
#include <cmath>

// Variabel untuk posisi dan arah kamera
float camX = 0.0f, camY = 3.0f, camZ = 8.0f;
float lookX = 0.0f, lookY = 2.0f, lookZ = 0.0f;
float camYaw = 0.0f, camPitch = 0.0f;
const float PI = 3.14159265358979323846f;

// Rotasi objek
float objRotX = 0.0f;
float objRotY = 0.0f;

// Mode proyeksi
enum ProjectionMode {
    // Ortogonal
    ORTHOGONAL_PLAN,      // Tampilan Rencana (Tampilan Atas)
    ORTHOGONAL_FRONT,     // Tampilan Depan
    ORTHOGONAL_SIDE,      // Tampilan Samping
    ORTHOGONAL_ISOMETRIC, // Isometrik
    ORTHOGONAL_DIMETRIC,  // Dimetrik
    ORTHOGONAL_TRIMETRIC, // Trimetrik
    // Perspektif
    PERSPECTIVE_1POINT,   // Perspektif 1 Titik
    PERSPECTIVE_2POINT,   // Perspektif 2 Titik
    PERSPECTIVE_3POINT,   // Perspektif 3 Titik
    PERSPECTIVE_FREE      // Kamera Bebas (Default)
};

ProjectionMode currentProjection = PERSPECTIVE_FREE;

// Fungsi untuk menggambar silinder
void drawCylinder(GLfloat radius, GLfloat height, GLint slices, GLint stacks) {
    GLUquadricObj *quadObj = gluNewQuadric();
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    gluQuadricNormals(quadObj, GLU_SMOOTH);
    gluQuadricTexture(quadObj, GL_TRUE);
    
    // Menggambar silinder
    gluCylinder(quadObj, radius, radius, height, slices, stacks);
    
    // Tutup atas
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, height);
    gluDisk(quadObj, 0.0, radius, slices, 1);
    glPopMatrix();
    
    // Tutup bawah
    glPushMatrix();
    glRotatef(180.0, 1.0, 0.0, 0.0);
    gluDisk(quadObj, 0.0, radius, slices, 1);
    glPopMatrix();
    
    gluDeleteQuadric(quadObj);
}

// Fungsi untuk menggambar daun oval seperti yang ditunjukkan dalam gambar
void drawNewLeaf() {
    glPushMatrix();
    
    // Matikan highlight specular untuk daun
    GLfloat no_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_specular);
    glMateriali(GL_FRONT, GL_SHININESS, 0);
    
    // Warna daun hijau lebih gelap dengan kilau yang lebih sedikit
    glColor3f(0.2f, 0.5f, 0.0f);
    
    // Bentuk daun oval
    glPushMatrix();
    glScalef(1.0f, 0.6f, 0.2f);
    glutSolidSphere(0.8, 30, 30);
    glPopMatrix();
    
    glPopMatrix();
}

// Fungsi untuk menggambar kelopak bunga biru
void drawBlueFlowerPetal() {
    // Matikan highlight specular untuk kelopak
    GLfloat no_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_specular);
    glMateriali(GL_FRONT, GL_SHININESS, 0);
    
    // Warna biru muda untuk kelopak yang lebih gelap dan kurang mengkilap
    glColor3f(0.3f, 0.5f, 0.8f);
    
    // Bentuk oval untuk kelopak
    glPushMatrix();
    glScalef(0.5f, 0.5f, 0.2f);
    glutSolidSphere(0.6, 20, 20);
    glPopMatrix();
}

// Fungsi untuk menggambar kelopak hijau (penutup belakang bunga)
void drawGreenSepals() {
    // Matikan highlight specular untuk kelopak hijau
    GLfloat no_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_specular);
    glMateriali(GL_FRONT, GL_SHININESS, 0);
    
    // Warna hijau untuk kelopak
    glColor3f(0.2f, 0.5f, 0.0f);
    
    // Menggambar kelopak hijau sebagai lingkaran datar
    glPushMatrix();
    glScalef(0.55f, 0.55f, 0.1f);
    glutSolidSphere(0.7, 20, 20);
    glPopMatrix();
}

// Fungsi untuk membuat vas bunga bulat
void drawRoundVase() {
    // Badan vas (bentuk bulat)
    glColor3f(0.4f, 0.2f, 0.6f); // Warna ungu untuk vas
    
    // Bagian bawah vas (bulat)
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glScalef(1.0f, 0.9f, 1.0f); // Sedikit rata secara vertikal
    glutSolidSphere(1.0, 30, 30);
    glPopMatrix();
    
    // Leher vas
    glPushMatrix();
    glTranslatef(0.0f, 0.8f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    drawCylinder(0.4, 0.5, 30, 10);
    glPopMatrix();
    
    // Tepi atas vas
    glPushMatrix();
    glTranslatef(0.0f, 1.3f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    drawCylinder(0.5, 0.1, 30, 5);
    glPopMatrix();
}

// Fungsi untuk membuat bunga baru dengan kelopak biru dan pusat kuning
void drawNewFlower() {
    // Batang bunga
    glColor3f(0.0f, 0.6f, 0.0f);
    
    // Atur material untuk batang
    GLfloat low_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, low_specular);
    glMateriali(GL_FRONT, GL_SHININESS, 10);
    
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    drawCylinder(0.05, 2.5, 10, 10);
    glPopMatrix();
    
    // Kepala bunga
    glPushMatrix();
    glTranslatef(0.0f, 2.5f, 0.0f);
    
    // Pusat bunga (kuning lebih terang)
    // Kurangi specular untuk pusat bunga
    glMaterialfv(GL_FRONT, GL_SPECULAR, low_specular);
    glMateriali(GL_FRONT, GL_SHININESS, 20);

    glColor3f(1.0f, 0.95f, 0.5f); // Kuning lebih terang
    glutSolidSphere(0.4, 25, 25);

    // Highlight pada pusat bunga
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.7f); // Highlight lebih terang
    glTranslatef(-0.1f, 0.1f, 0.2f);
    glScalef(0.3f, 0.3f, 0.1f);
    glutSolidSphere(0.5, 15, 15);
    glPopMatrix();
    
    // Kelopak bunga biru (8 kelopak) - mengelilingi di bidang XY
    for (int i = 0; i < 8; i++) {
        glPushMatrix();
        float angle = i * 45.0f; // Didistribusikan merata dalam 360 derajat (8 kelopak)
        glRotatef(angle, 0.0f, 0.0f, 1.0f); // Rotasi untuk posisi di sekitar bidang XY
        glTranslatef(0.6f, 0.0f, 0.0f); // Kelopak di samping inti bunga
        
        // Rotasi untuk membuat kelopak menghadap ke depan
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        
        drawBlueFlowerPetal();
        glPopMatrix();
    }
    glPopMatrix();
    
    // Tambahkan 2 daun oval di kiri dan kanan batang menghadap ke depan
    glPushMatrix();
    glTranslatef(0.7f, 1.0f, 0.0f);
    glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f); // Rotasi 180 derajat untuk menghadap ke depan
    drawNewLeaf();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.7f, 1.0f, 0.0f);
    glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f); // Rotasi 180 derajat untuk menghadap ke depan
    drawNewLeaf();
    glPopMatrix();
    
    // Reset material default setelah menggambar bunga
    GLfloat default_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, default_specular);
    glMateriali(GL_FRONT, GL_SHININESS, 50);
}

// Fungsi untuk memperbarui posisi kamera berdasarkan sudut
void updateCameraPosition() {
    // Konversi dari sudut ke arah pandang
    float dirX = sin(camYaw * PI / 180.0f) * cos(camPitch * PI / 180.0f);
    float dirY = sin(camPitch * PI / 180.0f);
    float dirZ = -cos(camYaw * PI / 180.0f) * cos(camPitch * PI / 180.0f);
    
    // Perbarui titik pandang
    lookX = camX + dirX;
    lookY = camY + dirY;
    lookZ = camZ + dirZ;
}

// Fungsi tampilan
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Posisikan kamera berdasarkan mode proyeksi
    switch(currentProjection) {
        case ORTHOGONAL_PLAN: // Tampilan Atas
            gluLookAt(0.0, 10.0, 0.0,  // Posisi kamera di atas objek
                    0.0, 0.0, 0.0,   // Melihat ke pusat objek
                    0.0, 0.0, -1.0); // Vektor atas yang disesuaikan
            break;
            
        case ORTHOGONAL_FRONT: // Tampilan Depan
            gluLookAt(0.0, 2.0, 10.0,  // Posisi kamera di depan objek
                    0.0, 2.0, 0.0,   // Melihat ke pusat objek
                    0.0, 1.0, 0.0);  // Vektor atas normal
            break;
            
        case ORTHOGONAL_SIDE: // Tampilan Samping
            gluLookAt(10.0, 2.0, 0.0,  // Posisi kamera di samping objek
                    0.0, 2.0, 0.0,   // Melihat ke pusat objek
                    0.0, 1.0, 0.0);  // Vektor atas normal
            break;
            
        case ORTHOGONAL_ISOMETRIC: // Isometrik
            gluLookAt(5.0, 5.0, 5.0,   // Posisi kamera (sudut sama)
                    0.0, 2.0, 0.0,   // Melihat ke pusat objek
                    0.0, 1.0, 0.0);  // Vektor atas normal
            break;
            
        case ORTHOGONAL_DIMETRIC: // Dimetrik
            gluLookAt(7.0, 5.0, 3.0,   // Posisi kamera (dua sudut sama)
                    0.0, 2.0, 0.0,   // Melihat ke pusat objek
                    0.0, 1.0, 0.0);  // Vektor atas normal
            break;
            
        case ORTHOGONAL_TRIMETRIC: // Trimetrik
            gluLookAt(7.0, 4.0, 5.0,   // Posisi kamera (tiga sudut berbeda)
                    0.0, 2.0, 0.0,   // Melihat ke pusat objek
                    0.0, 1.0, 0.0);  // Vektor atas normal
            break;
            
        case PERSPECTIVE_1POINT: // Perspektif 1 Titik
            gluLookAt(0.0, 2.0, 8.0,   // Kamera sejajar dengan objek
                    0.0, 2.0, 0.0,   // Melihat ke pusat objek
                    0.0, 1.0, 0.0);  // Vektor atas normal
            break;
            
        case PERSPECTIVE_2POINT: // Perspektif 2 Titik
            gluLookAt(5.0, 2.0, 8.0,   // Kamera miring ke samping
                    0.0, 2.0, 0.0,   // Melihat ke pusat objek
                    0.0, 1.0, 0.0);  // Vektor atas normal
            break;
            
        case PERSPECTIVE_3POINT: // Perspektif 3 Titik
            gluLookAt(6.0, 6.0, 6.0,   // Kamera dari sudut diagonal atas
                    0.0, 2.0, 0.0,   // Melihat ke pusat objek
                    0.2, 1.0, 0.2);  // Vektor atas miring
            break;
            
        case PERSPECTIVE_FREE: // Kamera Bebas (Default)
        default:
            // Perbarui posisi kamera
            updateCameraPosition();
            
            // Posisikan kamera
            gluLookAt(
                camX, camY, camZ,     // Posisi mata
                lookX, lookY, lookZ,  // Titik pusat
                0.0, 1.0, 0.0         // Vektor atas
            );
            break;
    }
    
    // Terapkan rotasi objek dalam mode perspektif bebas
    if (currentProjection == PERSPECTIVE_FREE) {
        glRotatef(objRotX, 1.0f, 0.0f, 0.0f);
        glRotatef(objRotY, 0.0f, 1.0f, 0.0f);
    }
    
    // Gambar vas bulat
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    drawRoundVase();
    glPopMatrix();
    
    // Gambar bunga baru
    glPushMatrix();
    glTranslatef(0.0f, 1.3f, 0.0f);
    drawNewFlower();
    glPopMatrix();
    
    // Tampilkan instruksi di layar
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // Tampilkan kontrol
    glRasterPos2i(10, 580);
    const char* controlText = "WASD: Gerak | QE: Naik/Turun | Panah: Rotasi | R: Reset";
    for (const char* c = controlText; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
    
    // Tampilkan mode proyeksi saat ini
    glRasterPos2i(10, 560);
    const char* projectionText;
    switch(currentProjection) {
        case ORTHOGONAL_PLAN:      projectionText = "Proyeksi: Ortogonal - Tampilan Atas"; break;
        case ORTHOGONAL_FRONT:     projectionText = "Proyeksi: Ortogonal - Tampilan Depan"; break;
        case ORTHOGONAL_SIDE:      projectionText = "Proyeksi: Ortogonal - Tampilan Samping"; break;
        case ORTHOGONAL_ISOMETRIC: projectionText = "Proyeksi: Ortogonal - Isometrik"; break;
        case ORTHOGONAL_DIMETRIC:  projectionText = "Proyeksi: Ortogonal - Dimetrik"; break;
        case ORTHOGONAL_TRIMETRIC: projectionText = "Proyeksi: Ortogonal - Trimetrik"; break;
        case PERSPECTIVE_1POINT:   projectionText = "Proyeksi: Perspektif - 1 Titik"; break;
        case PERSPECTIVE_2POINT:   projectionText = "Proyeksi: Perspektif - 2 Titik"; break;
        case PERSPECTIVE_3POINT:   projectionText = "Proyeksi: Perspektif - 3 Titik"; break;
        case PERSPECTIVE_FREE:     projectionText = "Proyeksi: Perspektif - Kamera Bebas"; break;
    }
    for (const char* c = projectionText; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
    
    // Tampilkan instruksi proyeksi
    glRasterPos2i(10, 540);
    const char* projInstructText = "Proyeksi: 1-6 (Ortogonal), 7-9 (Perspektif), 0 (Kamera Bebas)";
    for (const char* c = projInstructText; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
    
    glEnable(GL_LIGHTING);
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    glutSwapBuffers();
}

// Fungsi reshape
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    float aspect = (float)w / (float)h;
    
    // Konfigurasi proyeksi berdasarkan mode
    switch(currentProjection) {
        // Proyeksi Ortogonal
        case ORTHOGONAL_PLAN:
        case ORTHOGONAL_FRONT:
        case ORTHOGONAL_SIDE:
        case ORTHOGONAL_ISOMETRIC:
        case ORTHOGONAL_DIMETRIC:
        case ORTHOGONAL_TRIMETRIC:
            // Untuk proyeksi ortogonal, gunakan glOrtho
            // Parameter disesuaikan agar objek tampak dengan ukuran yang tepat
            glOrtho(-5.0 * aspect, 5.0 * aspect, -5.0, 5.0, 0.1, 100.0);
            break;
            
        // Proyeksi Perspektif
        case PERSPECTIVE_1POINT:
        case PERSPECTIVE_2POINT:
        case PERSPECTIVE_3POINT:
        case PERSPECTIVE_FREE:
        default:
            // Untuk proyeksi perspektif, gunakan gluPerspective
            gluPerspective(45.0f, aspect, 0.1f, 100.0f);
            break;
    }
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Fungsi keyboard
void keyboard(unsigned char key, int x, int y) {
    const float moveSpeed = 0.3f;
    
    // Hitung arah gerakan berdasarkan orientasi kamera saat ini
    float frontX = lookX - camX;
    float frontY = 0.0f; // Kita tidak ingin naik/turun saat bergerak maju/mundur
    float frontZ = lookZ - camZ;
    
    // Normalisasi vektor depan pada bidang XZ
    float length = sqrt(frontX*frontX + frontZ*frontZ);
    if (length > 0.0001f) {
        frontX /= length;
        frontZ /= length;
    }
    
    // Vektor kanan (tegak lurus dengan arah pandang)
    float rightX = frontZ;
    float rightZ = -frontX;
    
    switch (key) {
        case 27: // ESC
            exit(0);
            break;
        case 'w': // Maju
            camX += frontX * moveSpeed;
            camZ += frontZ * moveSpeed;
            lookX += frontX * moveSpeed;
            lookZ += frontZ * moveSpeed;
            break;
        case 's': // Mundur
            camX -= frontX * moveSpeed;
            camZ -= frontZ * moveSpeed;
            lookX -= frontX * moveSpeed;
            lookZ -= frontZ * moveSpeed;
            break;
        case 'a': // Kiri
            camX -= rightX * moveSpeed;
            camZ -= rightZ * moveSpeed;
            lookX -= rightX * moveSpeed;
            lookZ -= rightZ * moveSpeed;
            break;
        case 'd': // Kanan
            camX += rightX * moveSpeed;
            camZ += rightZ * moveSpeed;
            lookX += rightX * moveSpeed;
            lookZ += rightZ * moveSpeed;
            break;
        case 'q': // Naik
            camY += moveSpeed;
            lookY += moveSpeed;
            break;
        case 'e': // Turun
            camY -= moveSpeed;
            lookY -= moveSpeed;
            break;
        case 'r': // Reset posisi kamera dan objek
            camX = 0.0f;
            camY = 3.0f;
            camZ = 8.0f;
            lookX = 0.0f;
            lookY = 2.0f;
            lookZ = 0.0f;
            camYaw = 0.0f;
            camPitch = 0.0f;
            objRotX = 0.0f;
            objRotY = 0.0f;
            break;
            
        // Proyeksi Ortogonal
        case '1': currentProjection = ORTHOGONAL_PLAN; break;
        case '2': currentProjection = ORTHOGONAL_FRONT; break;
        case '3': currentProjection = ORTHOGONAL_SIDE; break;
        case '4': currentProjection = ORTHOGONAL_ISOMETRIC; break;
        case '5': currentProjection = ORTHOGONAL_DIMETRIC; break;
        case '6': currentProjection = ORTHOGONAL_TRIMETRIC; break;
        
        // Proyeksi Perspektif
        case '7': currentProjection = PERSPECTIVE_1POINT; break;
        case '8': currentProjection = PERSPECTIVE_2POINT; break;
        case '9': currentProjection = PERSPECTIVE_3POINT; break;
        case '0': currentProjection = PERSPECTIVE_FREE; break;
    }
    
    // Segarkan tampilan dan proyeksi
    glutPostRedisplay();
    reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

// Fungsi tombol khusus
void specialKeys(int key, int x, int y) {
    const float rotSpeed = 3.0f;
    
    switch (key) {
        case GLUT_KEY_UP:
            objRotX -= rotSpeed;
            break;
        case GLUT_KEY_DOWN:
            objRotX += rotSpeed;
            break;
        case GLUT_KEY_LEFT:
            objRotY -= rotSpeed;
            break;
        case GLUT_KEY_RIGHT:
            objRotY += rotSpeed;
            break;
    }
    
    glutPostRedisplay();
}

// Fungsi untuk pengaturan pencahayaan
void setupLighting() {
    // Aktifkan pencahayaan
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Atur properti cahaya
    GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat position[] = { 5.0f, 8.0f, 5.0f, 1.0f };
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    
    // Atur properti material
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMateriali(GL_FRONT, GL_SHININESS, 100);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Tugas Proyeksi Bunga 3D");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    
    glEnable(GL_DEPTH_TEST);
    setupLighting();
    
    glutMainLoop();
    return 0;
}
