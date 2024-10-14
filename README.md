# miniMinecraft

![ty](img/minicraft.gif)

In this project we built a mini-Minecraft game!

**Controls:**
W/A/S/D - player movement
SPACE - jump/float in water
E/Q - up/down in flight mode
F - toggle flight mode on/off
LMB/RBM - add/remove block
M - toggle third-person mode on/off
F1-5 - play emotes in third-person mode
P - adjust daynight cycle speed



**Milestone 1: Procedural Terrain Generation**

The main task I implemented is using noise function to procedurally generate terrain. Since noise function is only going to be used in procedural generation, I created a noisehelper to store all the noise functions that will be used. Later I separated the procedural generator into a different class to better use in later system features generation. The main function in procedural generator that the terrain class will use is the get height method, which will return a height value based on the x and z coordinates of the current block using noise functios. Terrain can then use the height to fill in the block type. 

The first thing I realized is that since we want to have the infinite expanding world map all randomly generated, the input fed into the noise function cannot be associated with any chunk, zone variable, which will then lead to repetition. Then the simplest method is to divide the x and z coordinates by a non-related constant, which can also serve to expand or compress the generated map into desired terrain. I used Perlin noise as the basis function for fbm to generate the mountain terrain height, and fbm noise as the basis function for worley noise, which factors in secondary cell distance, to generate grassland terrain height. The main difficulty of this part is choosing the right coefficients to tune the noise function to get to desire shape, e.g. the amplitude and persistence of fbm, the x and z coordinates divide constant, the random vector used in dot product etc. The last part is to blend different terrains together so that they can transition smoothly from one to another. I am having difficulty to understand which noise function to use and how to fine tune the noise function at the beginning, but ChatGPT is really helpful in giving me an example and I start from there. I first used Perlin noise as described in the description, but there were way too much mountain than grassland, so I used fbm again to scatter out the distribution.



**Milestone 2: Cave System and Post-process Shaders**

Cave system development is a more advanced application of Perlin noise that I used in milestone one, in which I extended the 2D Perlin noise to 3D Perlin noise and used a threshold to determine whether to keep the block. The result looked pretty nice, and I tuned the threshold a bit so that individual caves were not that far separated. Two things that I think I can improve in milestone three: first thing, I watched the Minecraft video presented by a Minecraft engineer posted on Canvas, and I am pretty interested in those two types of cave, the big chunk cheese cave and the narrow tunnel spaghetti cave. Currently, all the caves are cheese caves in shape, so I can add a separate noise function to create spaghetti caves and mix them so that caves get more interconnected. My second thought is that I can add texture to the surface area of the caves to make them look interesting.

The second part is the player physics in water and lava. Many thanks to Christine, who walked through the player code structure with me and pointed out some potential places to put in modifications so that I could finish it much faster. I added a new indicator variable into the player class to determine whether the player is in water or lava and used that variable to adjust the acceleration and velocity accordingly.

The third part, the post-process shader, took me the most time to debug. I finished the primary codec following HW4 ShaderFun's implementation, adding in framebuffer object, quat drawable and post-process shader into MyGL class, setting up the framebuffer and passing the texture accordingly. However, the output of the post-process shader looked like a magnified and shifted result of the texture. I spent a lot of time adjusting the viewport parameters and debugging the shader code, but no luck. Thanks to our TA Linda, who suggested that a resolution problem most likely caused the issue and that I should check the resize function. It turned out that after resizing the framebuffer, I also needed to destroy and create it again so that the textImage2D also had the correct size to take in the texture.



**Milestone 3: Desert Terrain, Day and Night Cycle, Shadow Mapping**

-Desert Terrain
This is an extension of the procedural terrain generation from part 1. I used Perlin FBM to generate the base desert noise and use smoothstep to filter out the sand dune pike. In part 1 I used perlin noise to blend the grassland and mountain together, which resulted in two biomes interconnected along a narrow tube shaped terrain. To better separate the desert and grassland-mountain terrain, I used worley noise to blend the three biomes together, and therefore the desert could have a shape of circle with large areas together instead of a narrow tube. I also tuned the smoothstep coefficients so that three biomes are almost equally separated.

-Day and Night Cycle
This is the part I spent most of the time tuning colors. ) I used the skybox example code that Adam provided to us and made changes based on that. First thing was to create a moving sun. The first thought that I had was that I did not want the sun to move around the player, which would cause the sun to move when the player moved, so instead I chose the sun to be at right above the origin at the beginning and rotate along the world z-axis. In order to make it look more realistic and also not to “bump” into the sun, the sun was moving along a circle with a distance of 5000 from the origin. Next step, add color into the sun and its dusk. I hard coded 11 stages for sun and its dusk throughout the entire day and mixed them together based on the current timestamp. Also, the corona size was changing along the day, making the sun have a larger area blending with the sky during sunrise and sunset. Next step was to add in sky color. I chose 9 colors for the sky and blended them together based on the timestamp as well as with the dusk color. The result was pretty nice I think. I liked the blinking stars effect that Adam created during class so I stole that from him, thanks Adam! I tuned the worley grid size and cutting threshold a little bit so that each star was bigger and more separated. As the last step I added in a moon which was located at the opposite side of the circle but rotated along the world x-axis. To avoid the case that moon and sun were both visible at the same time, which was possible since they lied on a single line and when the sun was at sunrise or sunset, we could also see half of the moon on the other side, I tuned the timestamp that moon became visible and blend its color with the sky color to have a blend-in blend-out effect. I used the same technique with the star color and sky color. Later to better test shadow mapping, I added a variable for total time elapsed so that we could adjust the time speed with keyboard input.

-Shadow Mapping
This is the part that was hard to debug. I followed the openGL tutorial of creating shadow mapping, and built the depth framebuffer to store z depth information texture, shadow shader to only pass out z depth information, and adjusted lambert shader to use the sun view projection matrix and compare current fragment depth from camera view with that from sun view to create shadows. Everything worked fine, except that the scene was always in shadow. I tested on the entire pipeline multiple times and tried to output the depth texture onto a quad to see the depth map result, and it turned out it was the view projection matrix of the sun that was setup incorrectly. I tried both the orthographic view projection matrix and perspective view projection matrix, but the perspective one always did not work, and for the orthographic matrix, the far clip plane was too small since the sun was located 5000 away from the origin, roughly the same for the player as well. After adjusting the far clip distance, I could see the shadow but they were shifting so I added in bias when comparing the depth in the lambert fragment shader. I tried to see the fast-forward result of shadows of the entire day through adjusting time speed, and I could still see weird shaped shadows for “mini-bump” on the desert. In order to solve this, I tried to add Poison Sampling to blur out the edge but it did not work, actually made it worse. I think for moving light source, shadow mapping was hard to give the best result considering the changing floating point error, so it is what’s currently looks like. Also, when adding water blocks into the scene, it would create strips like shadows along the water, the reason for that was the transparent blocks were also causing shadows even though we could not see them, so the solution was to not draw transparent blocks during depth rendering run.
