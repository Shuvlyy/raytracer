
## Scenes

Scene file yml syntax guide for raytracer

# Basic settings:

## 💾 Saving file:

First, you will need to specify where to save the rendered images:
```yml
# Basic test scene for Raytracer
outputDirectory: renders/
```

If the directory doesn't exist, it will be created by the program
WARNING: If none specified: The program will exit with an error and don't save the render but still compute it

## 🎥 Camera:

For the camera, you will need to specify basic settings:

```yml
camera:
  resolution:
    width: 640 # In pixels
    height: 480 # In pixels
    antialiasingSamples: 20 # Number of rays to cast for each pixel
    maxBounces: 45 # Number of bounces for a ray
  position: # Each in metters
    x: 0
    y: 0.3
    z: 1
  rotation: # In degrees
    yaw: 0
    pitch: 0
    roll: 0
  # In degrees
  fov: 80.0
  aperture: 2.8
  focalLength: 50.0
```

## Shape:

In order to create shapes, you need to create a `yml` list:

```yml
shapes:  
  - shape_name:
	  type: "TYPENAME" 
```

For each shape, i different syntax:

Note that when no `material` is specified, program falls back to `LAMBERTIAN` and logs a warning.
Same if no `color` is specified, program falls back to plain white and logs a warning.

```yml
# Plane
- plane_name:  
  type: "PLANE"
  axis: "Y"
  position: -0.5  # Offset on the axis
  color: "#CCCC33" # Color of the plane (optional)
  material: "METAL" # Material (optional)
  fuzz: 0.1 # Only for metal (optional)
```
```yml
# Sphere
- s1:
    type: "SPHERE"
    # Coordinates of the center
    x: -4
    y: 0
    z: -3
    radius: 0.2
```
```yml
# Triangle
- triangle:
    type: "TRIANGLE"
    p1:  # First vertex
      x: 0.8
      y: -0.8
      z: 0.8
    p2:  # Second vertex
      x: 0.3
      y: 0.2
      z: 0.5
    p3:  # Third vertex
      x: 0
      y: -1
      z: -0.5
    axis:
      x: 0.5
      y: 0.2
      z: 0
```

```yml
# Cylinder
- cylinder:
    type: "CYLINDER"
    x: 0.0
    y: 0.0
    z: 0.0
    axis: # Direction
      x: 0.0
      y: 1.0
      z: 0.0
    radius: 0.5 # Radius of the base
    height: 1.5 # Height of the cylinder
```

```yml
# Cone
- cone:
    type: "CONE"
    x: 0.0
    y: 0.0
    z: 0.0
    axis: # Axis direction (unit vector or direction)
      x: 0.0
      y: 1.0
      z: 0.0
    radius: 1.0 # Base radius
    height: 2.0 # Height of the cone
```

## Lights:

In order to create lights, you need to create a `yml` mapping:

```yml
lights:  
  type:
   # fields # 
```

For each light, i different syntax:

Note that again, if no `color` is specified, program falls back to plain white and logs a warning.

```yml
lights:
  ambient:
    color: "#FFFFFF"        # Light color (hex or RGB)
    intensity: 1.0          # Intensity (double)
```

If no direction is specified, programs falls back to (0, 0, 0) and logs a warning.
Note that you still need to mention `direction`.
```yml
lights:
  directional:
    color: "#FFFFFF"        # Light color
    direction:              # Normalized direction vector
      x: 0.0
      y: -1.0
      z: 0.0
    intensity: 1.0          # Intensity (currently fixed to 1 in code)
```

Also, if no position is specified, programs falls back to (0, 0, 0) and logs a warning.
```yml
lights:
  points:
    - name_1:
      x: 2.0
      y: 3.0
      z: 1.0
      intensity: 0.8        # Intensity of the point light
		- name_2:
      x: 0.8
      y: 0.5
      z: 0
      intensity: 0.3        # Intensity of the point light
```

### Made with 💜 by [Pierre M.](https://www.github.com/PierreMarguerie)
