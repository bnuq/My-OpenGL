#include <iostream>
#include <string>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;


/*
	VAO = Vertex Array Object => vertex 가 가지고 있는 정보가 무엇인지
							  => vertex attributes 를 저장하는 객체

	VBO = Vertex Buffer Object => vertex data itself

	그리고자 하는 모델마다, 이 객체들이 필요할 것
*/
GLuint VBO{}, VAO{}; //각 객체의 id 를 저장


//program object id
GLuint shader{};



//원래는 다른 파일에 정의해야 하지만, 일단 문자열 형태로 저장
//Vertex shader
static const char* vShader = "												  \n\
#version 330                                                                  \n\
                                                                              \n\
layout (location = 0) in vec3 pos;											  \n\
                                                                              \n\
void main()                                                                   \n\
{                                                                             \n\
    //정점의 위치 이동시키기 => 클립 공간 위치	                                  \n\
    gl_Position = vec4(0.8 * pos.x, 0.4 * pos.y, pos.z, 1.0);				  \n\
}";

//Fragment Shader
static const char* fShader = "                                                \n\
#version 330                                                                  \n\
                                                                              \n\
out vec4 colour;                                                              \n\
                                                                              \n\
void main()                                                                   \n\
{                                                                             \n\
	//레스터라이저에 의해 면 생성 => 프레그먼트 생성 => 프레그먼트의 색깔 결정     \n\
    colour = vec4(1.0, 0.0, 1.0, 1.0);                                        \n\
}";


//1. 삼각형을 그리는 함수
void CreateTriangle() {

	//그리고자 하는 삼각형의 정점의 위치좌표 => z 성분이 0, 2차원
	//왜 벡터로 하니까 안되지
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};
	
	
	/*
		Vertex Array Object 를 만들고 그 id 를 리턴

		1개만 만든다는 의미

		vao => cpu 에 존재하는 배열, attributes 설정을 저장하는 배열
			=> 또한 한개 이상의 vbo 객체를 가리킬 수 있다
			=> 어떤 vbo 를 사용하고 있을 때, 그 vbo 에 연결됐다고 말한다			
	*/
	glGenVertexArrays(1, &VAO);

	/*
		Bind the vertex array object

		앞서 정의한 vertex array objects 중 하나를 사용하기 위해서
		bind 한다, 사용하려는 vao 를 지정한다

		이제부터는 별말 없어도, VAO 라는 vertex array 를 사용하는 것이다
	*/
	glBindVertexArray(VAO);


		/*
			Generate buffer object names

			Buffer Object 를 만들고 그 id 를 리턴
				buffer object => data 를 가져와서 저장하는 역할
				이 버퍼에 데이터가 저장된다

			vbo => GPU 에 존재하는 배열, 모델의 정점들이 가지는 정보를 저장하는 배열

			자동으로 먼저 묶여있던 VAO 에 포함된다
			생성되는 VBO 는 자동으로 VAO 가 가리키게 된다
		*/
		glGenBuffers(1, &VBO);

		/*
			Bind a buffer object to the specified buffer binding point
			
			현재 사용하고 있는 VAO 에서 VBO 를 연결
			이제 별말 없어도 이 VBO 를 사용하는 것이다

			해당 buffer object 에 정점 데이터를 저장할 것이므로
			vertex array 와 연결되어 있으며, vertex attributes 로 해석해야 한다
			
			따라서 GL_ARRAY_BUFFER 설정 
			=> 정점에 대한 데이터를 저장하는 버퍼라는 것을 알리고
			=> 해당 퍼버를 GL_ARRAY_BUFFER 라는 이름과 묶어 => 해당 이름으로 가리킬 수 있다 = target
		*/
		glBindBuffer(GL_ARRAY_BUFFER, VBO);


			/*
				glBufferData
				creates a new data store for a buffer object
				and initializes a buffer object's data store
				the buffer object currently bound to target is used

				연결한 buffer object 에 data 를 저장
				읽고자 하는 data 는 vertices 배열

				연결된 buffer object => GL_ARRAY_BUFFER 라는 값으로 접근

				STATIC	=> 빈도수, 그리 많이 바꾸지 않는다
				DRAW	=> 그리는 데 사용된다?
			*/
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
			/*
				Define an array of generic vertex attribute data

				vao => vertex attributes 를 저장
				저장된 attributes 에 따라서, 	들어오는 데이터를 해석하는 방법을 지정

				그중 index 번째에 해석하는 방법을 지정
				
				index of the generic vertex attribute

				각 정점이 가지는 속성? 여기서는 x,y,z 3개
				각 속성 타입

				normalize 를 할 것인지
				건너뛰는 양?
				시작 offset
			*/
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			/*
				Enable or disable a generic vertex attribute array

				연결된 버퍼를 해석하는 방법을 지정
				vao 에 저장한 값들 중, index 번째에 지정해둔 attribute pointer 방법으로 해석하게 한다
			*/
			glEnableVertexAttribArray(0);

		//VBO buffer object 연결한 것을 unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	//VAO bind 했던 것을 다시 unbind
	glBindVertexArray(0);
}


//3. shader object 를 program object 에 연결하는 함수
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {

	//shader object 를 만든다음 id 저장
	GLuint theShader = glCreateShader(shaderType);

	//shader 내용을 가리키는 포인터
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	//shader 코드 길이
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	//shader object 에 shader 코드 내용을 연결? replace
	glShaderSource(theShader, 1, theCode, codeLength);

	//shader object 를 컴파일
	glCompileShader(theShader);


	//에러 부분 확인
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	//shader object 의 상태에 대해서, 조사?
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(theShader, 1024, NULL, eLog);
		fprintf(stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	//상태가 괜찮으면 컴파일한 shader object 를 program object 에 attach
	glAttachShader(theProgram, theShader);
}


//2. shader 를 program object 에 연결하고 생성
void CompileShaders() {

	/*
		An empty program object 를 리턴
		=> shader object 가 붙을 수 있다 => linked to create a program
	*/
	shader = glCreateProgram();
	//생성 에러 체크
	if (!shader) {
		printf("Failed to create shader\n");
		return;
	}

	//shader obejct 를 program object 에 연결한다
	//각 shader 의 종류를 정확하게 명시
	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	
	//에러 확인을 위한 부분
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	//Links a program object
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result); //여부 확인
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	//Validates a program object
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result); //여부 확인
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

}

int main()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		printf("GLFW initialisation failed!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core Profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow Forward Compatbility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the window
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer Size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	//Setup Viewport size
	//뷰포트를 만들고
	glViewport(0, 0, bufferWidth, bufferHeight);


		//삼각형 정점에 대해서 설정
		//vao, vbo 모두 정의하고 설정
		CreateTriangle();

		//모델에 적용될 셰이더를 만들고 컴파일 => link
		//사용하고자 하는 shader 에 대한 설정
		CompileShaders();

	// Loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get + Handle user input events
		glfwPollEvents();

		//Clear window
		//창의 기본 색깔 설정
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		//Program 실행
		//앞서 컴파일한 shaders 를 사용할 것이다
		glUseProgram(shader);

			/*
				VAO vertex array object 를 이제부터 사용한다 -
			
				해당 vertex array object 에 미리 데이터와 attribute 를 저장해 뒀어
				저장된 정보로 => 정점의 정보를 파악할 수 있다
			*/
			glBindVertexArray(VAO);

				/*
					render primitives from array data

					buffer object 에 들어있는 데이터를 
					정해진 attribute pointer 의 방법대로 읽은 다음에
					렌더링을 다음과 같이 진행해라~

					삼각형 = primitive
						0부터
						3개씩 나눠서

					VBO 가 GL_ARRAY_BUFFER 로 연결되어 있었기 때문에
					VBO 의 데이터를 사용한 걸까?
				*/
				glDrawArrays(GL_TRIANGLES, 0, 3);
		
			//Bind 한 정보 해제
			glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}