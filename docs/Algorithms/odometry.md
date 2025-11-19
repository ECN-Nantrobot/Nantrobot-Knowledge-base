# Odometry

Before implementing odometry, you need to be able to measure wheel rotations using encoders.  
See the dedicated page: [Encoders](../../Components/sensors/encoder.md)

## 📘 Theory Summary

Odometry is a method used to estimate a robot’s position **(x, y)** and orientation **θ** over time using wheel encoder measurements.  
For a differential drive robot, we assume:

- Two driven wheels (left and right)
- Pure rolling (no lateral slip)
- Encoders measuring the wheel rotations

### Differential-Drive Kinematic Model (Continuous View)

Under the pure rolling assumption, the robot’s linear and angular velocities are:

\[
\begin{cases}
v = \dfrac{r_r \dot{q}_r + r_l \dot{q}_l}{2} \\
\omega = \dfrac{r_r \dot{q}_r - r_l \dot{q}_l}{e}
\end{cases}
\]

and the continuous-time kinematics:

\[
\begin{cases}
\dot{x} = v \cos{\theta} \\
\dot{y} = v \sin{\theta} \\
\dot{\theta} = \omega
\end{cases}
\]

### Distance and Heading Increment (Discrete View)

In practice, we work with discrete time steps and encoder increments.  
From the wheel motions over step *k*:

\[
\Delta D_k = \frac{r_r \, \Delta q_{r,k} + r_l \, \Delta q_{l,k}}{2}
\]

\[
\Delta \theta_k = \frac{r_r \, \Delta q_{r,k} - r_l \, \Delta q_{l,k}}{e}
\]

with:

- \( r_r, r_l \) : right and left wheel radii (we usually assume they are equal: \( r_r = r_l = r \))
- \( \Delta q_{r,k}, \Delta q_{l,k} \) : wheel angle increments (radians) during step *k*  
- \( e \) : wheel baseline (distance between the two wheel contact points)

### Pose Update

Using these increments, the robot pose is updated at each step *k*:

\[
\begin{cases}
x_{k+1} = x_k + \Delta D_k \cos{\theta_k} \\
y_{k+1} = y_k + \Delta D_k \sin{\theta_k}
\end{cases}
\]

\[
\theta_{k+1} = \theta_k + \Delta \theta_k
\]

where:

- \( (x_k, y_k, \theta_k) \) is the pose at step *k*  
- \( \Delta D_k \) is the linear distance traveled during the interval  
- \( \theta_k \) is the heading at step *k*

In implementation, odometry integrates these discrete equations over time using encoder counts to update the robot’s estimated pose.

## 💻 From Encoders to Pose (Example Snippet)

The following code shows how to go from encoder counts to a pose update using the discrete equations defined above.

You will need to define an intialize a lot fo the variables used here.

```cpp title="odometry_update"
// Read encoder counts once (volatile → local copy)
long currentCountRight = encoderCountRight;
long currentCountLeft  = encoderCountLeft;

// Calculate delta counts for odometry
long deltaCountRight = currentCountRight - lastCountRight;
long deltaCountLeft  = currentCountLeft  - lastCountLeft;

// Save for next iteration
lastCountRight = currentCountRight;
lastCountLeft  = currentCountLeft;

// 1) Convert counts to wheel angle increments Δq (radians)
//    Δq = 2π * (Δcount / ENCODER_PULSES_PER_REVOLUTION)
float deltaQr = 2.0f * PI * (deltaCountRight / (float)ENCODER_PULSES_PER_REVOLUTION);
float deltaQl = 2.0f * PI * (deltaCountLeft  / (float)ENCODER_PULSES_PER_REVOLUTION);

// 2) Distance and heading increments (matches theory):
//    ΔD_k     = (r_r Δq_r + r_l Δq_l) / 2
//    Δθ_k     = (r_r Δq_r - r_l Δq_l) / e
float deltaD     = (WHEEL_RADIUS * deltaQr + WHEEL_RADIUS * deltaQl) / 2.0f;
float deltaTheta = (WHEEL_RADIUS * deltaQr - WHEEL_RADIUS * deltaQl) / WHEEL_BASE;

// 3) Pose update:
//    x_{k+1} = x_k + ΔD_k cos(θ_k)
//    y_{k+1} = y_k + ΔD_k sin(θ_k)
//    θ_{k+1} = θ_k + Δθ_k
robotX     += deltaD * cos(robotTheta);
robotY     += deltaD * sin(robotTheta);
robotTheta += deltaTheta;

// Optional: normalize θ to [-π, π]
while (robotTheta >  PI) robotTheta -= 2.0f * PI;
while (robotTheta < -PI) robotTheta += 2.0f * PI;

```