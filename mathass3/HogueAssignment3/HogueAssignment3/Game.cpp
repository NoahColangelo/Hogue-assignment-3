#include "Game.h"

#include <string>

Game::Game()
{
	updateTimer = new Timer();
}

Game::~Game()
{
	delete updateTimer;
}

void Game::initializeGame()
{
	// OpenGL will not draw triangles hidden behind other geometry
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	//player.setWorldPosition(Vector3(-3.0f, 8.0f, -5.0f));
	//player.setLocalScale(0.2f);

	float aspect = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);
	camera.perspective(60.0f, aspect, 1.0f, 1000.0f);
	//camera.orthographic(-10, 10, -10, 10, -100, 100);
	camera.setWorldPosition(vec3(0.0f, 0.0f, 5.0f));
	camera.setLocalRotationAngleX(camera.getLocalRotationAngleX() - 0.2f);
}

void Game::update()
{
	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;
	drawTime += deltaTime;

	camera.setWorldPosition(MathLibCore::lerp( camera.getWorldPosition(), player.getWorldPosition(), deltaTime * 3));
	camera.update(deltaTime);
}

void Game::draw()
{
		// Completely clear the Back-Buffer before doing any work.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef _DEBUG
		// New imgui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplFreeGLUT_NewFrame();

		// Update imgui widgets
		imguiDraw();

		// Render imgui
		ImGui::Render();
#endif

		// Update imgui draw data
		glUseProgram(GL_NONE);
#ifdef _DEBUG

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

		// Commit the Back-Buffer to swap with the Front-Buffer and be displayed on the monitor.
		glutSwapBuffers();
		drawTime = 0.0f;
}

void Game::imguiDraw()
{
	{
		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.


	}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
}

void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
#ifdef _DEBUG

	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[key] = true;
#endif

	switch (key)
	{
	case 32:
		if (camera.getProjType() == ProjectionType::Perspective)
			camera.orthographic(-10.0f, 10.0f, 10.0f, -10.0f, -100.0f, 100.0f);
		else
		{
			float aspect = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);
			camera.perspective(60.0f, aspect, 1.0f, 1000.0f);
		}
		break;
	case 27: // the escape key
	//case 'q': // the 'q' key
		exit(1);
		break;
	}
}

void Game::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
#ifdef _DEBUG

	ImGuiIO& io = ImGui::GetIO();
	io.KeysDown[key] = false;
#endif

	switch(key)
	{
	case 32: // the space bar
		break;
	case 27: // the escape key
	//case 'q': // the 'q' key
		exit(1);
		break;
	}
#ifdef _DEBUG

#endif
}

void Game::mouseClicked(int button, int state, int x, int y)
{
#ifdef _DEBUG

	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2((float)x, (float)y);
	//io.MouseDown[button] = state;

	if(state == GLUT_DOWN) 
	{
		switch(button)
		{
		case GLUT_LEFT_BUTTON:
			io.MouseDown[0] = true;
			break;
		case GLUT_RIGHT_BUTTON:
			io.MouseDown[1] = true;
			break;
		case GLUT_MIDDLE_BUTTON:
			io.MouseDown[2] = true;
			break;
		}
	}
	else
	{
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
			io.MouseDown[0] = false;
			break;
		case GLUT_RIGHT_BUTTON:
			io.MouseDown[1] = false;
			break;
		case GLUT_MIDDLE_BUTTON:
			io.MouseDown[2] = false;
			break;
		}
	}
#endif
}

/*
 * mouseMoved(x,y)
 * - this occurs only when the mouse is pressed down
 *   and the mouse has moved.  you are given the x,y locations
 *   in window coordinates (from the top left corner) and thus 
 *   must be converted to screen coordinates using the screen to window pixels ratio
 *   and the y must be flipped to make the bottom left corner the origin.
 */
void Game::mouseMoved(int x, int y)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos = ImVec2((float)x, (float)y);
}
