#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include "loadShaders.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "SOIL.h"


GLuint VaoId, VboId, ProgramId, myMatrixLocation, texture;
glm::mat4 myMatrix, resizeMatrix, backgroundMatrix;

float xMin = -160.0f, xMax = 160.0f, yMin = -100.0f, yMax = 100.0f;

float rectangle1X = -1.0f;  // Initial X position of the first rectangle
float rectangle2X = -1.6f;   // Initial X position of the second rectangle
float rectangle2Y = -0.10f;
float rectangle3X = 0.23f; //police
float rectangle3Y = 0.4f; //police
float speed = 0.040f;  // Speed of movement for 1
float speed2 = 0.056f;  // Speed of movement for 1
float speed3 = 0.058f;
float angle = 0;					//	Unghiul de rotire al patratului;
float tx = 0; float ty = 0; float auxtx; float auxangle;
float rotation = 0.0f;
float rotation3 = 0.0f;


void LoadTexture(const char* photoPath)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    //	Desfasurarea imaginii pe orizonatala/verticala in functie de parametrii de texturare;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height;
    unsigned char* image = SOIL_load_image(photoPath, &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void UpdateRectangles() {
    rectangle1X += speed;
    rectangle2X += speed2;

    if (rectangle2X > -0.8f && rectangle2X < -0.5f) {
        rectangle2X += speed2;
        rectangle2Y += 0.058f;
        rotation = 9.0f;
    }


    if (rectangle2X > -0.5f && rectangle2X < 0.1f) {
        rectangle2X += speed2;
        rotation = 0.0f;
    }

    if (rectangle2X > 0.1f && rectangle2X < 0.3f) {
        rectangle2X += speed2;
        rectangle2Y -= 0.008f;
        rotation = -10.0f;
    }

    if (rectangle2X > 0.3f) {
        rectangle2X += 0.008f;
        rectangle2Y = -0.10f;
        rotation = 0.0f;
    }

    if (rectangle2X > 0.5f && rectangle2X < 0.95f) {
        rotation3 = -25.5f;
        rectangle3X += speed3-0.02f;
        rectangle3Y -= 0.075f;
    }

    if (rectangle3X > 0.48f) {
        rotation3 = 0.0f;
        rectangle3X += 0.058f;
    }
}



void CreateShaders() {
    ProgramId = LoadShaders("03_05_Shader.vert", "03_05_Shader.frag");
    glUseProgram(ProgramId);
}

void CreateVBO(void)
{
    // Coordonatele varfurilor;
    static const GLfloat Vertices[] =
    {
        //	Cele 4 varfuri din colturi; 
        xMin, yMin, 0.0f, 1.0f,    	0.0f, 0.8f, 0.0f,  	0.0f, 0.0f, 
        xMax, yMin, 0.0f, 1.0f,	    0.0f, 0.8f, 0.0f,	0.0f, 0.0f,
        xMax, yMax, 0.0f, 1.0f,	    0.0f, 0.4f, 0.0f,	0.0f, 0.0f,
        xMin, yMax, 0.0f, 1.0f,	    0.0f, 0.4f, 0.0f,	0.0f, 0.0f,

        //	Varfuri pentru axe;
        xMin, 0.0f, 0.0f, 1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
        xMax,  0.0f, 0.0f, 1.0f,	1.0f, 1.0f, 1.0f,	0.0f, 0.0f,


        //sosea
        xMin, -30.0f, 0.0f, 1.0f,	    0.0f, 0.0f, 0.0f,	0.0f, 0.0f,
        xMax,  -30.0f, 0.0f, 1.0f,	    0.0f, 0.0f, 0.0f,	0.0f, 0.0f,
        xMax, 30.0f, 0.0f, 1.0f,	    0.0f, 0.0f, 0.0f,	0.0f, 0.0f,
        xMin, 30.0f, 0.0f, 1.0f,	    0.0f, 0.0f, 0.0f,	0.0f, 0.0f,


        xMin, 0.0f, 0.0f, 1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
        xMin,  0.0f, 0.0f, 1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,


        -10.0f, -7.0f,  0.0f,  1.0f,	    0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
        20.0f, -7.0f,  0.0f,  1.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
        20.0f,  7.0f,  0.0f,  1.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
        -10.0f, 7.0f,  0.0f,  1.0f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,

        -10.0f, -7.0f,  0.0f,  1.0f,	  0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
        20.0f, -7.0f,  0.0f,  1.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
        20.0f,  7.0f,  0.0f,  1.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
        -10.0f, 7.0f,  0.0f,  1.0f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,


        xMin, 1.0f, 0.0f, 1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
        xMax, 1.0f, 0.0f, 1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,

        xMin, -1.0f, 0.0f, 1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
        xMax,  -1.0f, 0.0f, 1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,

        xMin, -29.0f, 0.0f, 1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
        xMax,  -29.0f, 0.0f, 1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,

        xMin, 29.0f, 0.0f, 1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
        xMax,  29.0f, 0.0f, 1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,


        -10.0f, 15.0f, 0.0f, 1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
        10.0f,  15.0f, 0.0f, 1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,

         -10.0f, -15.0f, 0.0f, 1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
         10.0f,  -15.0f, 0.0f, 1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,

         10.0f, -15.0f, 0.0f, 1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
         6.0f, -11.0f, 0.0f, 1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,

         10.0f, -15.0f, 0.0f, 1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,	
         6.0f, -19.0f, 0.0f, 1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,


         -10.0f, 15.0f, 0.0f, 1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,	
         -6.0f, 11.0f, 0.0f, 1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,

         -10.0f, 15.0f, 0.0f, 1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,	
         -6.0f, 19.0f, 0.0f, 1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,	

        65.0f , 35.0f,  0.0f,  1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,	
        80.0f, 35.0f,  0.0f,  1.0f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f,
        80.0f,  50.0f,  0.0f,  1.0f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
        65.0f, 50.0f,  0.0f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 1.0f,

        25.0f , 35.0f,  0.0f,  1.0f,	    1.0f, 1.0f, 1.0f,	0.0f, 0.0f,	
        10.0f, 35.0f,  0.0f,  1.0f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f,
        10.0f, 50.0f,  0.0f,  1.0f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
        25.0f, 50.0f,  0.0f,  1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 1.0f,

        //semn circulatie

         -80, -35, 0.0f, 1.0f,    	0.0f, 0.0f, 0.0f,  	0.0f, 0.0f,	
        -110, -35, 0.0f, 1.0f,	    0.0f, 0.0f, 0.0f,	1.0f, 0.0f,	
        -110, -60, 0.0f, 1.0f,	    0.0f, 0.0f, 0.0f,	1.0f, 1.0f,	
        -80, -60, 0.0f, 1.0f,	    0.0f, 0.0f, 0.0f,	0.0f, 1.0f,	

        //masina politie
        -10.0f, -7.0f,  0.0f,  1.0f,	  0.0f, 1.0f, 0.0f,	0.0f, 0.0f,	
        20.0f, -7.0f,  0.0f,  1.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
        20.0f,  7.0f,  0.0f,  1.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
        -10.0f, 7.0f,  0.0f,  1.0f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,

    };



    // Transmiterea datelor prin buffere;

    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);

    glGenBuffers(1, &VboId);                                                        //  Generarea bufferului si indexarea acestuia catre variabila VboId;
    glBindBuffer(GL_ARRAY_BUFFER, VboId);                                           //  Setarea tipului de buffer - atributele varfurilor;
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
    //  Se asociaza atributul (1 =  culoare) pentru shader;
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
    //  Se asociaza atributul (2 =  texturare) pentru shader;
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));


}

void DestroyShaders(void)
{
    glDeleteProgram(ProgramId);
}

void DestroyVBO(void)
{
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VboId);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VaoId);
}

void Cleanup() {
    DestroyShaders();
    DestroyVBO();
}

void Initialize() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);		//  Culoarea de fond a ecranului;
    CreateVBO();								//  Trecerea datelor de randare spre bufferul folosit de shadere;
    CreateShaders();							//  Initilizarea shaderelor;
    //	Instantierea variabilelor uniforme pentru a "comunica" cu shaderele;
    myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
    glUniform1i(glGetUniformLocation(ProgramId, "ok"), 0);

    //	Dreptunghiul "decupat"; 
    resizeMatrix = glm::ortho(xMin, xMax, yMin, yMax);
    backgroundMatrix = glm::ortho(xMin, xMax, yMin, yMax); // Matrice pentru fundal și linia albă

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RenderBackground() {
    myMatrix = backgroundMatrix;
    glUniform1i(glGetUniformLocation(ProgramId, "ok"), 0);

    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, glm::value_ptr(myMatrix));
    glPointSize(10.0);
    glDrawArrays(GL_POLYGON, 0, 4);
    glDrawArrays(GL_POLYGON, 6, 4);
    //glLineWidth(2.0);
    //glDrawArrays(GL_LINES, 4, 2);
    glLineWidth(2.0);
    glDrawArrays(GL_LINES, 10, 2);
    glLineWidth(2.0);
    glDrawArrays(GL_LINES, 20, 2);
    glLineWidth(2.0);
    glDrawArrays(GL_LINES, 22, 2);
    glLineWidth(2.0);
    glDrawArrays(GL_LINES, 24, 2);
    glLineWidth(2.0);
    glDrawArrays(GL_LINES, 26, 2);
    glLineWidth(3.0);
    glDrawArrays(GL_LINES, 28, 2);
    glLineWidth(3.0);
    glDrawArrays(GL_LINES, 30, 2);
    glLineWidth(3.0);
    glDrawArrays(GL_LINES, 32, 2);
    glLineWidth(3.0);
    glDrawArrays(GL_LINES, 34, 2);
    glLineWidth(3.0);
    glDrawArrays(GL_LINES, 36, 2);

    glLineWidth(3.0);
    glDrawArrays(GL_LINES, 38, 2);

    glDisable(GL_POINT_SMOOTH);

}

void RenderFunction() {

    glClear(GL_COLOR_BUFFER_BIT);			//  Se curata ecranul OpenGL pentru a fi desenat noul continut;

    RenderBackground();  // Desenează fundalul
    UpdateRectangles();

    LoadTexture("semncirc.png");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    ////	Transmiterea variabilelor uniforme pentru MATRICEA DE TRANSFORMARE, PERSPECTIVA si PROIECTIE spre shadere;
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
    ////	Transmiterea variabilei uniforme pentru TEXTURARE spre shaderul de fragmente;
    glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);
    glUniform1i(glGetUniformLocation(ProgramId, "ok"), 1);
    glDrawArrays(GL_POLYGON, 48, 4);


    LoadTexture("copacdesus.png");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    ////	Transmiterea variabilelor uniforme pentru MATRICEA DE TRANSFORMARE, PERSPECTIVA si PROIECTIE spre shadere;
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
    ////	Transmiterea variabilei uniforme pentru TEXTURARE spre shaderul de fragmente;
    glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);
    glUniform1i(glGetUniformLocation(ProgramId, "ok"), 1);
    glDrawArrays(GL_POLYGON, 40, 4);

    LoadTexture("copacdesus.png");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    ////	Transmiterea variabilelor uniforme pentru MATRICEA DE TRANSFORMARE, PERSPECTIVA si PROIECTIE spre shadere;
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
    ////	Transmiterea variabilei uniforme pentru TEXTURARE spre shaderul de fragmente;
    glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);
    glUniform1i(glGetUniformLocation(ProgramId, "ok"), 1);
    glDrawArrays(GL_POLYGON, 44, 4);


    myMatrix = resizeMatrix;
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, glm::value_ptr(myMatrix));
    myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(rectangle1X, -0.10f, 0.0f)) * resizeMatrix;
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, glm::value_ptr(myMatrix));
    LoadTexture("pinkcar2.png");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    ////	Transmiterea variabilelor uniforme pentru MATRICEA DE TRANSFORMARE, PERSPECTIVA si PROIECTIE spre shadere;
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
    ////	Transmiterea variabilei uniforme pentru TEXTURARE spre shaderul de fragmente;
    glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);
    glUniform1i(glGetUniformLocation(ProgramId, "ok"), 1);
    glDrawArrays(GL_POLYGON, 12, 4);



    myMatrix = resizeMatrix;
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, glm::value_ptr(myMatrix));
    myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(rectangle2X, rectangle2Y, 0.0f)) * resizeMatrix;
    myMatrix = glm::rotate(myMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, glm::value_ptr(myMatrix));
    LoadTexture("pinkcar.png");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    ////	Transmiterea variabilelor uniforme pentru MATRICEA DE TRANSFORMARE, PERSPECTIVA si PROIECTIE spre shadere;
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
    ////	Transmiterea variabilei uniforme pentru TEXTURARE spre shaderul de fragmente;
    glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);
    glUniform1i(glGetUniformLocation(ProgramId, "ok"), 1);
    glDrawArrays(GL_POLYGON, 16, 4);

    //masina politie
    myMatrix = resizeMatrix;
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, glm::value_ptr(myMatrix));
    myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(rectangle3X, rectangle3Y, 0.0f)) * resizeMatrix;
    myMatrix = glm::rotate(myMatrix, glm::radians(rotation3), glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, glm::value_ptr(myMatrix));
    LoadTexture("masinapolitie.png");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    ////	Transmiterea variabilelor uniforme pentru MATRICEA DE TRANSFORMARE, PERSPECTIVA si PROIECTIE spre shadere;
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
    ////	Transmiterea variabilei uniforme pentru TEXTURARE spre shaderul de fragmente;
    glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);
    glUniform1i(glGetUniformLocation(ProgramId, "ok"), 1);
    glDrawArrays(GL_POLYGON, 52, 4);


    glutSwapBuffers();	//	Inlocuieste imaginea deseneata in fereastra cu cea randata; 
    glFlush();
}

void Timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(10, Timer, 0);  
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Depasire");

    glewInit();

    Initialize();
    glutDisplayFunc(RenderFunction);
    glutCloseFunc(Cleanup);
    glutTimerFunc(0, Timer, 0);
    glutMainLoop();

    return 0;
}