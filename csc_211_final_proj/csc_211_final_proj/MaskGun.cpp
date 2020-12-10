#include "pch.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "MaskGun.h"
#include "Player.h"
#include "TextureHolder.h"
#include "Mask.h"
#include "Pickup.h"

using namespace sf;
using namespace std;

int main()
{
	// Here is the instance of TextureHolder
	TextureHolder holder;

	// The game will always be in one of four states
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };
	// Start with the GAME_OVER state
	State state = State::GAME_OVER;


	// Get the screen resolution and create an SFML window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y),
		"Mask Gun", Style::Fullscreen);

	// Create a an SFML View for the main action
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// Here is our clock for timing everything
	Clock clock;
	// How long has the PLAYING state been active
	Time gameTimeTotal;

	// Where is the mouse in relation to world coordinates
	Vector2f mouseWorldPosition;
	// Where is the mouse in relation to screen coordinates
	Vector2i mouseScreenPosition;

	// Create an instance of the Player class
	Player player;

	// The boundaries of the arena
	IntRect arena;

	// Create the background
	VertexArray background;
	// Load the texture for our background vertex array
	Texture textureBackground = TextureHolder::GetTexture(
		"graphics/mask_gun_background_sheet.png");

	// Prepare for a group of customers
	int numCustomers;
	int numCustomersUnMasked;
	Customer* customers = NULL;

	// 100 masks should do
	Mask masks[100];
	int currentMask = 0;
	int masksSpare = 24;
	int masksInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	// When was the fire button last pressed?
	Time lastPressed;

	// Hide the mouse pointer and replace it with crosshair
	window.setMouseCursorVisible(true);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);

	// Create a couple of pickups
	Pickup healthPickup(1);
	Pickup ammoPickup(2);

	// Power up bool
	Pickup fireRatePowerUp();

	// Instructions bool
	bool instructions = false;

	// About the game
	int score = 0;
	int highScore = 0;

	// For the home/game over screen
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);

	// For instructions screen
	Sprite spriteInstructions;
	Texture textureInstructions = TextureHolder::GetTexture("graphics/instructions_background.png");
	spriteInstructions.setTexture(textureInstructions);
	spriteInstructions.setPosition(0, 0);

	// Create a view for the HUD
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// Create a view for the instructions view
	View instructionsView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// Load the font
	Font font;
	font.loadFromFile("fonts/Marlboro.ttf");

	// Paused
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setOutlineThickness(5);
	pausedText.setOutlineColor(Color::Black);
	pausedText.setPosition(400, 400);
	pausedText.setString("Press Enter \nto continue");

	// Game Over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(Color::White);
	gameOverText.setOutlineThickness(5);
	gameOverText.setOutlineColor(Color::Black);
	gameOverText.setPosition(250, 850);
	gameOverText.setString("Press Enter to Play");

	// Leveling up
	int wave = 0;
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setOutlineThickness(5);
	levelUpText.setOutlineColor(Color::Black);
	levelUpText.setPosition(150, 250);
	stringstream levelUpStream;
	levelUpStream <<
		//"1- Increased rate of fire" <<
		//"\n2- Increased clip size(next reload)" <<
		"Congratulations!"
		"\nLevel passed! Choose Upgrade:"
		"\n1- Increased max health" <<
		"\n2- Increased run speed" <<
		"\n3- More and better health pickups" <<
		"\n4- More frequent mask powerups";
	levelUpText.setString(levelUpStream.str());

	// Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(55);
	ammoText.setFillColor(Color::White);
	ammoText.setOutlineThickness(5);
	ammoText.setOutlineColor(Color::Black);
	ammoText.setPosition(200, 980);

	// Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setOutlineThickness(5);
	scoreText.setOutlineColor(Color::Black);
	scoreText.setPosition(20, 0);

	// Load the high score from a text file
	ifstream inputFile("gamedata/scores.txt");
	if (inputFile.is_open())
	{
		inputFile >> highScore;
		inputFile.close();
	}

	// High Score
	Text highScoreText;
	highScoreText.setFont(font);
	highScoreText.setCharacterSize(55);
	highScoreText.setFillColor(Color::White);
	highScoreText.setOutlineThickness(5);
	highScoreText.setOutlineColor(Color::Black);
	highScoreText.setPosition(1400, 0);
	stringstream s;
	s << "High Score:" << highScore;
	highScoreText.setString(s.str());

	// Customers remaining
	Text customersRemainingText;
	customersRemainingText.setFont(font);
	customersRemainingText.setCharacterSize(55);
	customersRemainingText.setFillColor(Color::White);
	customersRemainingText.setOutlineThickness(5);
	customersRemainingText.setOutlineColor(Color::Black);
	customersRemainingText.setPosition(1500, 980);
	customersRemainingText.setString("Customers: 100");

	// Wave number
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setOutlineThickness(5);
	waveNumberText.setOutlineColor(Color::Black);
	waveNumberText.setPosition(1250, 980);
	waveNumberText.setString("Wave: 0");

	// Instructions
	Text instructionsText;
	instructionsText.setFont(font);
	instructionsText.setCharacterSize(55);
	instructionsText.setFillColor(Color::White);
	instructionsText.setOutlineThickness(5);
	instructionsText.setOutlineColor(Color::Black);
	instructionsText.setPosition(1250, 850);
	instructionsText.setString("Press G for Guide");

	Text homeText;
	homeText.setFont(font);
	homeText.setCharacterSize(55);
	homeText.setFillColor(Color::White);
	homeText.setOutlineThickness(5);
	homeText.setOutlineColor(Color::Black);
	homeText.setPosition(1250, 850);
	homeText.setString("Press H for Home");

	// Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Green);
	healthBar.setPosition(450, 980);

	// When did we last update the HUD?
	int framesSinceLastHUDUpdate = 0;
	// What time was the last update
	Time timeSinceLastUpdate;
	// How often (in frames) should we update the HUD
	int fpsMeasurementFrameInterval = 1000;

	// Prepare the hit sound
	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("sound/hit.wav");
	Sound hit;
	hit.setBuffer(hitBuffer);

	// Prepare the shoot sound
	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/shoot.wav");
	Sound shoot;
	shoot.setBuffer(shootBuffer);

	// Prepare the powerup sound
	SoundBuffer powerupBuffer;
	powerupBuffer.loadFromFile("sound/powerup.wav");
	Sound powerup;
	powerup.setBuffer(powerupBuffer);

	// Prepare the pickup sound
	SoundBuffer pickupBuffer;
	pickupBuffer.loadFromFile("sound/pickup.wav");
	Sound pickup;
	pickup.setBuffer(pickupBuffer);

	// The main game loop
	while (window.isOpen())
	{
		/*
		************
		Handle input
		************
		*/

		// Handle events
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				// View instructions
				if (event.key.code == Keyboard::G &&
					state == State::GAME_OVER &&
					instructions == false)
				{
					instructions = true;
				}

				else if (event.key.code == Keyboard::H &&
					state == State::GAME_OVER &&
					instructions == true)
				{
					instructions = false;
				}

				// Pause a game while playing
				else if (event.key.code == Keyboard::Return &&
					state == State::PLAYING)
				{
					state = State::PAUSED;
				}

				// Restart while paused
				else if (event.key.code == Keyboard::Return &&
					state == State::PAUSED)
				{
					state = State::PLAYING;
					// Reset the clock so there isn't a frame jump
					clock.restart();
				}

				// Start a new game while in GAME_OVER state
				else if (event.key.code == Keyboard::Return &&
					state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;					
					wave = 0;
					score = 0;

					// Prepare the gun and masks for next game
					currentMask = 0;
					masksSpare = 24;
					masksInClip = 6;
					clipSize = 6;
					fireRate = 1;

					// Reset the player's stats
					player.resetPlayerStats();

				}
				// Spin and zoom the world

				if (Keyboard::isKeyPressed(Keyboard::Left))
				{
					mainView.rotate(.5f);
				}

				if (Keyboard::isKeyPressed(Keyboard::Right))
				{
					mainView.rotate(-.5f);
				}

				if (Keyboard::isKeyPressed(Keyboard::Up))
				{
					mainView.zoom(.99f);
				}

				if (Keyboard::isKeyPressed(Keyboard::Down))
				{
					mainView.zoom(1.01f);
				}
				// End spinning and zooming


			}
		}// End event polling


		 // Handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// Handle controls while playing
		if (state == State::PLAYING)
		{
			// Handle the pressing and releasing of the WASD keys
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
			}
			else
			{
				player.stopUp();
			}

			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
			}
			else
			{
				player.stopDown();
			}

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();
			}
			else
			{
				player.stopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
			}
			else
			{
				player.stopRight();
			}

			// Fire a mask
			if (Mouse::isButtonPressed(       Mouse::Left))
			{

				if (gameTimeTotal.asMilliseconds()
					- lastPressed.asMilliseconds()
					> 1000 / fireRate && masksInClip > 0)
				{

					// Pass the centre of the player and the centre of the crosshair
					// to the shoot function
					masks[currentMask].shoot(
						player.getCenter().x, player.getCenter().y,
						mouseWorldPosition.x, mouseWorldPosition.y);

					currentMask++;
					if (currentMask > 99)
					{
						currentMask = 0;
					}
					lastPressed = gameTimeTotal;
					shoot.play();
					// masksInClip--;
				}

			}// End fire a mask

		}// End WASD while playing

		// Handle the leveling up state
		if (state == State::LEVELING_UP)
		{
			// Handle the player leveling up

			if (wave != 0) 
			{
				if (event.key.code == Keyboard::Num1)
				{
					// Increase health
					player.upgradeHealth();
					state = State::PLAYING;
				}

				if (event.key.code == Keyboard::Num2)
				{
					// Increase speed
					player.upgradeSpeed();
					state = State::PLAYING;
				}

				if (event.key.code == Keyboard::Num3)
				{
					healthPickup.upgrade();
					state = State::PLAYING;
				}

				if (event.key.code == Keyboard::Num4)
				{
					ammoPickup.upgrade();
					state = State::PLAYING;
				}
			}
			if (wave == 0)
			{
				state = State::PLAYING;
			}


			if (state == State::PLAYING)
			{
				// Increase the wave number
				wave++;

				// Prepare the level
				arena.width = 500 * (wave * .75);
				arena.height = 500 * (wave * .75);
				arena.left = 0;
				arena.top = 0;

				// Pass the vertex array by reference 
				// to the createBackground function
				int tileSize = createBackground(background, arena);

				// Spawn the player in the middle of the arena
				player.spawn(arena, resolution, tileSize);

				// Configure the pick-ups
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

				// Create a horde of customers
				numCustomers = 5 * (wave * 1.5);

				// Delete the previously allocated memory (if it exists)
				delete[] customers;
				customers = createCustomers(numCustomers, arena);
				numCustomersUnMasked = numCustomers;

				// Set customer speed
				for (int i = 0; i < numCustomers; i++)
				{
					customers[i].setSpeed(wave);
				}

				// Play the powerup sound
				powerup.play();

				// Reset the clock so there isn't a frame jump
				clock.restart();
			}
		}// End leveling up

		 /*
		 ****************
		 UPDATE THE FRAME
		 ****************
		 */
		if (state == State::PLAYING)
		{
			// fire rate power up
			ammoPickup.powerUp();

			if (ammoPickup.getPowerUp() == true)
			{
				fireRate = 5;
			}

			if (ammoPickup.getPowerUp() == false) {
				fireRate = 1;
			}

			// Update the delta time
			Time dt = clock.restart();
			// Update the total game time
			gameTimeTotal += dt;
			// Make a decimal fraction of 1 from the delta time
			float dtAsSeconds = dt.asSeconds();

			// Where is the mouse pointer
			mouseScreenPosition = Mouse::getPosition();

			// Convert mouse position to world coordinates of mainView
			mouseWorldPosition = window.mapPixelToCoords(
				Mouse::getPosition(), mainView);

			// Set the crosshair to the mouse world location
			spriteCrosshair.setPosition(mouseWorldPosition);

			// Update the player
			player.update(dtAsSeconds, Mouse::getPosition());

			// Make a note of the players new position
			Vector2f playerPosition(player.getCenter());

			// Make the view center around the player				
			mainView.setCenter(player.getCenter());

			// Loop through each Customer and update them

			for (int i = 0; i < numCustomers; i++)
			{
				if (customers[i].isUnMasked())
				{
					customers[i].update(dt.asSeconds(), playerPosition);
				}
			}

			// Update any masks that are in-flight
			for (int i = 0; i < 100; i++)
			{
				if (masks[i].isInFlight())
				{
					masks[i].update(dtAsSeconds);
				}
			}

			// Update the pickups
			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);

			// Collision detection
			// Have any customers been shot?
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < numCustomers; j++)
				{
					if (masks[i].isInFlight() &&
						customers[j].isUnMasked())
					{
						if (masks[i].getPosition().intersects
						(customers[j].getPosition()))
						{
							// Stop the mask
							masks[i].stop();

							// Register the hit 
							if (customers[j].hit()) {
								
								score += 10;
								if (score > highScore)
								{
									highScore = score;
								}

								numCustomersUnMasked--;

								// When all the customers are masked (again)
								if (numCustomersUnMasked == 0) {
									state = State::LEVELING_UP;
								}
							}
						}
					}
				}
			}// End customer being shot

			// Have any customers touched the player			
			for (int i = 0; i < numCustomers; i++)
			{
				if (player.getPosition().intersects
				(customers[i].getPosition()) && customers[i].isUnMasked())
				{

					if (player.hit(gameTimeTotal))
					{					
						hit.play();
					}

					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;
						instructions = false;
						ofstream outputFile("gamedata/scores.txt");
						outputFile << highScore;
						outputFile.close();

					}
				}
			}// End player touched

			// Has the player touched health pickup
			if (player.getPosition().intersects
			(healthPickup.getPosition()) && healthPickup.isSpawned())
			{
				player.increaseHealthLevel(healthPickup.gotIt());
				// Play a sound
				pickup.play();

			}

			// Has the player touched ammo pickup
			if (player.getPosition().intersects
			(ammoPickup.getPosition()) && ammoPickup.isSpawned())
			{
				masksSpare += ammoPickup.gotIt();

			}

			// size up the health bar
			healthBar.setSize(Vector2f(player.getHealth() * 3, 70));

			// Increment the amount of time since the last HUD update
			timeSinceLastUpdate += dt;
			// Increment the number of frames since the last HUD calculation
			framesSinceLastHUDUpdate++;
			// Calculate FPS every fpsMeasurementFrameInterval frames
			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
			{

				// Update game HUD text
				stringstream ssAmmo;
				stringstream ssScore;
				stringstream ssHighScore;
				stringstream ssWave;
				stringstream ssCustomersUnMasked;

				// Update the ammo text
				ssAmmo << masksInClip << "/" << masksSpare;
				ammoText.setString(ssAmmo.str());

				// Update the score text
				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());

				// Update the high score text
				ssHighScore << "High Score:" << highScore;
				highScoreText.setString(ssHighScore.str());

				// Update the wave
				ssWave << "Wave:" << wave;
				waveNumberText.setString(ssWave.str());

				// Update the high score text
				ssCustomersUnMasked << "Customers:" << numCustomersUnMasked;
				customersRemainingText.setString(ssCustomersUnMasked.str());

				framesSinceLastHUDUpdate = 0;
				timeSinceLastUpdate = Time::Zero;
			}// End HUD update

		}// End updating the scene

		 /*
		 **************
		 Draw the scene
		 **************
		 */

		if (state == State::PLAYING)
		{
			window.clear();

			// set the mainView to be displayed in the window
			// And draw everything related to it
			window.setView(mainView);

			// Draw the background
			window.draw(background, &textureBackground);

			// Draw the customers
			for (int i = 0; i < numCustomers; i++)
			{
				window.draw(customers[i].getSprite());
			}

			for (int i = 0; i < 100; i++)
			{
				if (masks[i].isInFlight())
				{
					window.draw(masks[i].getShape());
				}
			}

			// Draw the player
			window.draw(player.getSprite());

			// Draw the pickups is currently spawned
			if (ammoPickup.isSpawned())
			{
				window.draw(ammoPickup.getSprite());
			}
			if (healthPickup.isSpawned())
			{
				window.draw(healthPickup.getSprite());
			}

			//Draw the crosshair
			window.draw(spriteCrosshair);

			// Switch to the HUD view
			window.setView(hudView);

			// Draw all the HUD elements
			
			window.draw(scoreText);
			window.draw(highScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(customersRemainingText);
		}

		if (state == State::LEVELING_UP)
		{
			if (wave == 0) 
			{
				window.draw(spriteGameOver);
			}

			else 
			{
				window.draw(spriteGameOver);
				window.draw(levelUpText);
			}
			
		}

		if (state == State::PAUSED)
		{
			window.draw(pausedText);
		}

		if (state == State::GAME_OVER && !instructions)
		{
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(highScoreText);
			window.draw(instructionsText);
		}

		if (state == State::GAME_OVER && instructions)
		{
			window.setView(instructionsView);
			window.draw(spriteInstructions);
			window.draw(homeText);
			window.draw(gameOverText);
		}

		window.display();

	}// End game loop

	return 0;
}