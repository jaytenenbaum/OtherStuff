-----------------------------------------------------------------------------------------------------------------------------------
------------------------- acceleration of player	-------------------
-----------------------------------------------------------------------------------------------------------------------------------


function love.update(dt)
	totalTime=totalTime+ dt
	shootingCoolDown=shootingCoolDown+dt
	-- update the dynamics of world
	world:update(dt)
	local dx=0
	local dy=0
	-- whether user clicked and button this iteration
	local clicked=0
	-- update x,y axes of player according to pressed button
	if love.keyboard.isDown("left") and objects.player.x>15 then objects.player.x = objects.player.x - dt * objects.player.speed; dx=dx-1 ;clicked=1 end
	if love.keyboard.isDown("right") and objects.player.x<635 then objects.player.x = objects.player.x + dt * objects.player.speed; dx=dx+1;clicked=1 end
	if love.keyboard.isDown("up") and objects.player.y>15 then objects.player.y = objects.player.y - dt * objects.player.speed; dy=dy-1;clicked=1 end
	if love.keyboard.isDown("down") and objects.player.y<635 then objects.player.y = objects.player.y + dt * objects.player.speed; dy=dy+1;clicked=1 end
	-- of user moved, save the last move
	if not (dx==0 and dy==0) then
		lastx=dx
		lasty=dy
	end
	-- if nothing clicked, update the location and decelerate 
	if clicked ==0 then
		if lastx>0 and objects.player.x<635 then objects.player.x = objects.player.x + dt * objects.player.speed end
		if lastx<0 and objects.player.x>15 then objects.player.x = objects.player.x - dt * objects.player.speed end
		if lasty>0 and objects.player.y<635 then objects.player.y = objects.player.y + dt * objects.player.speed end
		if lasty<0 and objects.player.y>15 then objects.player.y = objects.player.y - dt * objects.player.speed end
		if objects.player.speed>0 then objects.player.speed=objects.player.speed-1.8*acc end
	-- accelerate
	else
		if objects.player.speed<250 then objects.player.speed=objects.player.speed+acc end
	end
	-- if user shot and was moving in a direction beforehand, and cool- down is over
	if love.keyboard.isDown(' ') and not (lastx==0 and lasty==0) and shootingCoolDown>0.1 then
		-- reset cool- down
		shootingCoolDown=0
		-- find user direction
		local direction = math.atan2(lasty,lastx)
		-- create a new bullet in the direction of user, starting from user
		table.insert(objects.bullets, {
			x = objects.player.x,
			y = objects.player.y,
			dir = direction,
			speed = 800
		})
	end
	--update all the bullets movement and collisions
	updateBullets(dt)
	-- update whether enemies hit wall
	updateEnemyWallCollision()
	-- check user to enemy collision to detect if user lost
	checkPlayerEnemyCollision()
	
	-- if no more enemies, user wins
	if table.getn(objects.enemies) == 0 then
		won = 1
	end
	
	-- normalize enemy speeds
	enemySpeedWatchDog()
end
 
function love.draw()
	local i, o
	-- if game is over, don't draw ant player graphics
	if won == 0 then
		-- if still in game
		if lost ==0 then
			-- draw player:
			love.graphics.setColor(0, 0, 0)
			love.graphics.circle('fill', objects.player.x, objects.player.y, 15, 100)
			-- draw bullets:
			love.graphics.setColor(255, 255, 255, 224)
			for i, o in ipairs(objects.bullets) do
				love.graphics.circle('fill', o.x, o.y, 5, 100)
			end
		end
		-- draw enemies
		for i, o in ipairs(objects.enemies) do
			love.graphics.setColor(o.colorR, o.colorG, o.colorB)
			love.graphics.circle("fill",o.body:getX(), o.body:getY(),15,100)
		end
	end
	-- if lost only show output
	if lost == 1 then
		love.graphics.setColor(0, 255, 0, 255)
		love.graphics.print("You Lose! :(", 200,300,0,3,3)
	end
	-- if won only show output
	if won == 1 then
		love.graphics.setColor(0, 255, 0, 255)
		love.graphics.print("You Win! :)", 200,300,0,3,3)
	end
	love.graphics.setColor(0, 255, 0, 255)
	if totalTime<1 then love.graphics.print("Instructions: move with arrows, shoot with spacebar.", 200,300) end
end
 
function love.load()
	totalTime=0
	-- the rate of acceleration
	acc =1
	-- the last direction in x axis where the player travelled in. +1 for right, -1 for left, 0 for up/down.
	lastx=0
	-- the last direction in y axis where the player travelled in. +1 for down, -1 for up, 0 for right/left.
	lasty=0
	-- whether the player has lost yet
	lost = 0
	-- whether the player has won yet
	won = 0
	-- time to wait for cool down for shot
	shootingCoolDown=0
	
	--set world
	love.graphics.setBackgroundColor(50, 75, 125)
	love.physics.setMeter(64)
	world = love.physics.newWorld(0, 0, true)
	
	objects = {}
	-- player representing the user
	objects.player = {
		x = 325,
		y = 325,
		speed = 0,
		heat = 0,
		heatp = 0.1
	}
	-- the bullets which the user shoots
	objects.bullets = {}
	-- the enemies of user
	objects.enemies = {}

	-- create 10 enemies
	math.randomseed( os.time() )
	i=0
	for i=1,10 do 	
		addNewEnemy()
	end
	
	-- total amount of enemies killed
	totalCollisions = 0
	love.window.setMode(650, 650) 
end

function addNewEnemy()
	new ={}
	-- set new coordinates
	local newx = math.random(30,620)
	local newy = math.random(30,620)
	--make sure new enemy spawns far from player
	while  (objects.player.x - newx)^2+(objects.player.y - newy)^2 < (15+80)^2 do
		newx = math.random(30,620)
		newy = math.random(30,620)
	end
	-- dynamic ball, pretty standard
	new.body = love.physics.newBody(world,newx, newy, "dynamic")
	new.shape = love.physics.newCircleShape(15)
	new.fixture = love.physics.newFixture(new.body, new.shape, 1) -- Attach fixture to body and give it a density of 1.
	--make ellastic
	new.fixture:setRestitution(1)
	-- set random color
	new.colorR = math.random(0,255)
	new.colorG = math.random(0,255)
	new.colorB = math.random(0,255)
	table.insert(objects.enemies, new)
		
	-- give it initial random speed
	local x = (math.random()-0.5)*2
	local y = (math.random()-0.5)*2
	local direction  = math.atan2(y,x)
	new.body:setLinearVelocity(math.cos(direction)*100,math.sin(direction)*100)
end

function updateBullets(dt)
	-- update bullets locations based on speeds:
	local i, o
	-- iterate over bullets
	for i, o in ipairs(objects.bullets) do
		-- update x,y using speed and direction
		o.x = o.x + math.cos(o.dir) * o.speed * dt
		o.y = o.y + math.sin(o.dir) * o.speed * dt
		
		-- remove bullet if out of bounds
		if (o.x < 5) or (o.x > love.graphics.getWidth() -5)
		or (o.y < 5) or (o.y > love.graphics.getHeight() -5) then
			table.remove(objects.bullets, i)
		end
	end
	
	--bullets hitting enemies
	for i, bul in ipairs(objects.bullets) do
		for j, enem in ipairs(objects.enemies) do
			--detect collision between bullet and enemy
			if (bul.x - enem.body:getX())*(bul.x - enem.body:getX())+(bul.y - enem.body:getY())*(bul.y - enem.body:getY()) < (5+15)*(5+15) then
				-- remove bullet and enemy
				table.remove(objects.bullets,i)
				table.remove(objects.enemies,j)
				totalCollisions = totalCollisions+1
				-- depending on parity, spawn new enemy or not
				if totalCollisions % 2 == 0 then
					addNewEnemy()
				end
			end
		end
	end
end

-- update the new velocity of an enemy, in case he hits a wall
function updateEnemyWallCollision()
	for i, enem in ipairs(objects.enemies) do
		local xvel
		local yvel
		xvel,yvel = enem.body:getLinearVelocity()
		-- check if user is out of x bound
		if enem.body:getX()<15 or enem.body:getX()>635 then
			enem.body:setLinearVelocity( -xvel, yvel )
		end
		-- check if user is out of y bound
		if enem.body:getY()<15 or enem.body:getY()>635 then
			enem.body:setLinearVelocity(xvel, -yvel )
		end
	end

end

-- check if player hit an enemy (game over)
function checkPlayerEnemyCollision()
	for i, enem in ipairs(objects.enemies) do
		-- user is touching the enemy
		if (objects.player.x - enem.body:getX())*(objects.player.x - enem.body:getX())+(objects.player.y - enem.body:getY())*(objects.player.y - enem.body:getY()) < (15+15)*(15+15) then
			lost = 1
		end
	end
end

-- make sure enemy velocity is constant
function enemySpeedWatchDog()
	for i, enem in ipairs(objects.enemies) do
		local xvel
		local yvel
		xvel,yvel = enem.body:getLinearVelocity()
		-- normalized x and y velocities
		local xvelNormalized
		local yvelNormalized
		-- define them
		xvelNormalized = xvel/ ((xvel^2+yvel^2)^0.5)
		yvelNormalized = yvel/ ((xvel^2+yvel^2)^0.5)
		-- set them as new velocities
		enem.body:setLinearVelocity( xvelNormalized*100, yvelNormalized*100 )
	end
end