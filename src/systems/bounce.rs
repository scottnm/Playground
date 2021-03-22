use amethyst::{
    core::timing::Time,
    core::{SystemDesc, Transform},
    derive::SystemDesc,
    ecs::{Join, Read, ReadStorage, System, SystemData, World, WriteStorage},
    input::{InputHandler, StringBindings},
};

use crate::pong::{Ball, Paddle, Side, ARENA_HEIGHT};

struct Rect {
    left: f32,
    right: f32,
    top: f32,
    bottom: f32,
}

impl Rect {
    fn expand(&self, expansion: f32) -> Rect {
        Rect {
            left: self.left - expansion,
            bottom: self.bottom - expansion,
            right: self.right + expansion,
            top: self.top + expansion,
        }
    }

    fn includes_point(&self, (x, y): (f32, f32)) -> bool {
        x >= self.left && x <= self.right && y >= self.bottom && y <= self.top
    }
}

#[derive(SystemDesc)]
pub struct BounceSystem;

impl<'s> System<'s> for BounceSystem {
    type SystemData = (
        WriteStorage<'s, Ball>,
        ReadStorage<'s, Paddle>,
        ReadStorage<'s, Transform>,
    );

    fn run(&mut self, (mut balls, paddles, transforms): Self::SystemData) {
        // Check for collisions and bounce
        for (ball, ball_transform) in (&mut balls, &transforms).join() {
            let ball_x = ball_transform.translation().x;
            let ball_y = ball_transform.translation().y;

            // Bounce at the top or bottom of the pane
            if (ball_y <= ball.radius && ball.velocity[1] < 0.0
                || ball_y >= ARENA_HEIGHT - ball.radius && ball.velocity[1] > 0.0)
            {
                ball.velocity[1] = -ball.velocity[1];
            }

            // Bounce at the paddles
            for (paddle, paddle_transform) in (&paddles, &transforms).join() {
                let paddle_x = paddle_transform.translation().x - (paddle.width * 0.5);
                let paddle_y = paddle_transform.translation().y - (paddle.height * 0.5);
                let paddle_rect = Rect {
                    left: paddle_x,
                    right: paddle_x + paddle.width,
                    bottom: paddle_y,
                    top: paddle_y + paddle.height,
                };
                let collision_rect = paddle_rect.expand(ball.radius);
                if collision_rect.includes_point((ball_x, ball_y)) {
                    // Only register a collision if we are moving towards the paddle
                    // when we collide
                    let register_collision = match paddle.side {
                        Side::Left => ball.velocity[0] < 0.0,
                        Side::Right => ball.velocity[0] > 0.0,
                    };

                    if register_collision {
                        ball.velocity[0] = -ball.velocity[0]
                    }
                }
            }
        }
    }
}
