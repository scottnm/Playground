use amethyst::{
    core::{SystemDesc, Transform},
    derive::SystemDesc,
    ecs::{Join, Read, ReadStorage, System, SystemData, World, WriteStorage},
    input::{InputHandler, StringBindings},
};

use crate::pong::{Paddle, Side, ARENA_HEIGHT, ARENA_WIDTH, PADDLE_HEIGHT};

#[derive(SystemDesc)]
pub struct PaddleSystem;

impl<'s> System<'s> for PaddleSystem {
    type SystemData = (
        WriteStorage<'s, Transform>,
        ReadStorage<'s, Paddle>,
        Read<'s, InputHandler<StringBindings>>,
    );

    fn run(&mut self, (mut transforms, paddles, input): Self::SystemData) {
        for (paddle, transform) in (&paddles, &mut transforms).join() {
            let movement = match paddle.side {
                Side::Left => input.axis_value("left_paddle"),
                Side::Right => input.axis_value("right_paddle"),
            };
            if let Some(mv_amount) = movement {
                let scaled_amount = 1.2 * mv_amount as f32;
                let paddle_y = transform.translation().y;

                const ARENA_UPPER_BOUND: f32 = ARENA_HEIGHT - PADDLE_HEIGHT * 0.5;
                const ARENA_LOWER_BOUND: f32 = PADDLE_HEIGHT * 0.5;

                let new_paddle_y = (paddle_y + scaled_amount)
                    .min(ARENA_UPPER_BOUND)
                    .max(ARENA_LOWER_BOUND);

                transform.set_translation_y(new_paddle_y);
            }
        }
    }
}
