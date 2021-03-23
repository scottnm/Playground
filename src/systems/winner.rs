use amethyst::{
    core::timing::Time,
    core::{SystemDesc, Transform},
    derive::SystemDesc,
    ecs::{Join, Read, ReadStorage, System, SystemData, World, WriteStorage},
    input::{InputHandler, StringBindings},
};

use crate::pong::{Ball, ARENA_HEIGHT, ARENA_WIDTH};

#[derive(SystemDesc)]
pub struct WinnerSystem;

impl<'s> System<'s> for WinnerSystem {
    type SystemData = (WriteStorage<'s, Ball>, WriteStorage<'s, Transform>);

    fn run(&mut self, (mut balls, mut transforms): Self::SystemData) {
        for (ball, transform) in (&mut balls, &mut transforms).join() {
            let ball_x = transform.translation().x;
            let did_hit = if ball_x <= ball.radius {
                println!("Player 2 scores!");
                true
            } else if ball_x >= ARENA_WIDTH - ball.radius {
                println!("Player 1 scores!");
                true
            } else {
                false
            };

            // Reset the ball and reverse it's direction
            if did_hit {
                ball.velocity[0] = -ball.velocity[0];
                transform.set_translation_x(ARENA_WIDTH / 2.0);
                transform.set_translation_y(ARENA_HEIGHT / 2.0);
            }
        }
    }
}
