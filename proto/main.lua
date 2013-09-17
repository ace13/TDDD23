local world = nil
local planets = {}
local ships = {}
local bullets = {}

local turn = 1
local fire = false

function walls()

	local function wall( x, y, w, h )
		local b = love.physics.newBody( world, x + w / 2, y + h / 2, "static")
		local s = love.physics.newRectangleShape( w, h )
		local f = love.physics.newFixture( b, s, 1 )

		f:setRestitution( 0.9 )
		f:setUserData( "world" )
	end

	local width = love.graphics.getWidth()
	local height = love.graphics.getHeight()

	-- Top
	wall( 0, -1, width, 1 )
	-- Right
	wall( width, 0, 1, height )
	-- Bottom
	wall( 0, height, width, 1 )
	-- Left
	wall( -1, 0, 1, height )

end

function planet( x, y )
	local b = love.physics.newBody( world, x, y, "static" )
	local s = love.physics.newCircleShape( 50 )
	local f = love.physics.newFixture( b, s, 1 )

	f:setUserData( "planet" )
	f:setFriction( 2 )

	planets[#planets+1] = b
end

function ship( x, y, ang )
	local ang = (ang or 0) * 3.1415/180
	local b = love.physics.newBody( world, x, y, "dynamic" )
	local s = love.physics.newPolygonShape( 5, 5, 0, -5, -5, 5 )
	local f = love.physics.newFixture( b, s, 1 )

	f:setCategory( #ships+2 )
	b:setSleepingAllowed( true )
	f:setUserData( "ship" )
	b:setAngle(ang)

	ships[#ships+1] = { }
	ships[#ships].Body = b
	ships[#ships].Shape = s
	ships[#ships].Data = { }
	ships[#ships].Data.Health = 100
	ships[#ships].Data.Id = #ships+1
end

function bullet( ship, direction, force )
	local b = love.physics.newBody( world, ship.Body:getX(), ship.Body:getY(), "dynamic" )
	local s = love.physics.newCircleShape( 1 )
	local f = love.physics.newFixture( b, s, 1 )

	f:setMask( ship.Data.Id )
	b:setBullet( true )
	b:setLinearVelocity( math.cos(direction) * force, math.sin(direction) * force )
	f:setUserData( "bullet" )
	f:setRestitution( 0.9 )

	bullets[#bullets+1] = { }
	bullets[#bullets].Body = b
	bullets[#bullets].Shape = s
	bullets[#bullets].Ship = ship
	bullets[#bullets].Time = 8

end

function collision( a, b, contact )

	-- Custom collision solving goes here
	if ( a:getUserData() == "bullet" or b:getUserData() == "bullet" ) and ( a:getUserData() ~= "world" and b:getUserData() ~= "world" ) then
		if a:getUserData() == "bullet" then
			local body = a:getBody()

			for i = 1,#bullets do
				if bullets[i].Body == body then
					bullets[i] = nil
					break
				end
			end

			body:destroy()
		end

		if b:getUserData() == "bullet" then
			local body = b:getBody()

			for i = 1,#bullets do
				if bullets[i].Body == body then
					bullets[i] = nil
					break
				end
			end

			body:destroy()
		end

		local ship = nil
		if a:getUserData() == "ship" then
			for i = 1,#ships do
				if ships[i].Body == a:getBody() then
					ship = ships[i]
					break
				end
			end
		end
		if b:getUserData() == "ship" then
			for i = 1,#ships do
				if ships[i].Body == b:getBody() then
					ship = ships[i]
					break
				end
			end
		end

		if ship then
			ship.Data.Health = ship.Data.Health - 25
		end
	end

end

function love.load()
	love.graphics.setCaption( "Gameplay prototype in unsuitable engine" )

	world = love.physics.newWorld( 0, 0, true )
	world:setCallbacks( collision )

	walls()

	planet(100, 400)
	planet(600, 100)
	planet(550, 500)

	ship(130, 360, 45)
	ship(600, 150, 180)
	ship(550, 450)

end

local tot = 0

function love.update(dt)
	world:update( dt )

	tot = tot + dt

	if tot > 1 / 200 then
		tot = 0

		local looper = ships
		for i = 1,2 do
			if i > 1 and #bullets > 0 then
				looper = bullets
			elseif i > 1 then
				break
			end

			for _, s in ipairs(looper) do
				for _, p in ipairs(planets) do
					local b1 = s.Body
					local b2 = p

					local r = math.sqrt((b1:getX()-b2:getX())^2 + (b1:getY()-b2:getY())^2)

					if r <= 200 then
						local dX = (b1:getX() - b2:getX()) / r
						local dY = (b1:getY() - b2:getY()) / r

						s.Body:applyForce( -dX, -dY )
					end
				end

				if looper == bullets then
					s.Time = s.Time - dt
					if s.Time < 0 then
						looper[_] = nil
					end
				elseif s.Data.Health == 0 then
					looper[_] = nil
					break
				end
			end
		end
	end

	if love.mouse.isDown( "l" ) and not fire then
		local dX, dY = ships[turn].Body:getLinearVelocity()
		local speed = math.sqrt(dX*dX + dY*dY)

		if speed < 2 and #bullets == 0 then
			local mx, my = love.mouse.getPosition()

			local sx = ships[turn].Body:getX()
			local sy = ships[turn].Body:getY()

			local diff = math.min(math.sqrt((sx-mx)^2 + (sy-my)^2), 100)

			bullet( ships[turn], math.atan2(my - sy, mx - sx), diff * 5 )

			turn = turn + 1
		end

		fire = true
	elseif not love.mouse.isDown( "l" ) and fire then
		fire = false
	end

	if turn > #ships then
		turn = 1
	end
end

function clerp( t )
	local r = { 255, 0, 0 }
	local y = { 255, 255, 0 }
	local g = { 0, 255, 0 }

	local function lerp( a, b, t ) return a + (b-a) * t end
	local function cler( a, b, t ) return lerp(a[1], b[1], t), lerp(a[2], b[2], t), lerp(a[3], b[3], t) end 

	if t > 50 then
		return cler(y, g, (t-50)/50)
	else
		return cler(r, y, t/50)
	end
end

function love.draw()
	for _, b in ipairs(planets) do
		love.graphics.setColor( 128, 128, 255, 25 )
		love.graphics.circle( "fill", b:getX(), b:getY(), 200, 36 )
		love.graphics.setColor( 255, 255, 255, 255 )
		love.graphics.circle( "line", b:getX(), b:getY(), 50, 36 )
	end

	if #ships >= turn and turn > 0 and #bullets == 0 then
		b = ships[turn]

		local dX, dY = b.Body:getLinearVelocity()
		local speed = math.sqrt(dX*dX + dY*dY)

		if speed < 2 then
			love.graphics.setColor( 0, 255, 0, 50 )
			love.graphics.circle( "fill", b.Body:getX(), b.Body:getY(), 100, 30 )
		end
	end

	love.graphics.setColor( 255, 255, 255, 255 )

	for _, b in ipairs(ships) do
		love.graphics.circle( "line", b.Body:getX(), b.Body:getY(), 1, 3)
		love.graphics.polygon( "line", b.Body:getWorldPoints( b.Shape:getPoints() ) )

		local R,G,B = clerp( b.Data.Health )

		love.graphics.setColor( R, G, B, 255 )
		local diff = 20 - ((b.Data.Health / 100) * 20)
		love.graphics.rectangle( "fill", b.Body:getX() - diff + 10, b.Body:getY() - 15, diff - 20, 4 )
		love.graphics.setColor( 255, 255, 255, 255 )
		love.graphics.rectangle( "line", b.Body:getX() - 10, b.Body:getY() - 15, 20, 4 )
	end

	for _, b in ipairs(bullets) do
		love.graphics.circle( "line", b.Body:getX(), b.Body:getY(), 1, 3)
	end

end
