use amethyst::{
    core::timing::Time,
    core::{SystemDesc, Transform},
    derive::SystemDesc,
    ecs::{
        Entity, Join, Read, ReadExpect, ReadStorage, System, SystemData, World, Write, WriteStorage,
    },
    input::{InputHandler, StringBindings},
    ui::UiText,
};

use crate::pong::{Ball, ScoreBoard, ScoreText, ARENA_HEIGHT, ARENA_WIDTH};

#[derive(SystemDesc)]
pub struct WinnerSystem;

impl<'s> System<'s> for WinnerSystem {
    type SystemData = (
        WriteStorage<'s, Ball>,
        WriteStorage<'s, Transform>,
        WriteStorage<'s, UiText>,
        Write<'s, ScoreBoard>,
        ReadExpect<'s, ScoreText>,
    );

    fn run(&mut self, system_data: Self::SystemData) {
        let (mut balls, mut transforms, mut ui_text, mut scores, score_text) = system_data;
        for (ball, transform) in (&mut balls, &mut transforms).join() {
            fn update_score(score: &mut i32, ui_entity: Option<&mut UiText>) {
                *score = (*score + 1).min(999);
                if let Some(text) = ui_entity {
                    text.text = score.to_string();
                }
            }

            let ball_x = transform.translation().x;
            let did_hit = if ball_x <= ball.radius {
                update_score(
                    &mut scores.p2_score,
                    ui_text.get_mut(score_text.p2_score_text),
                );
                true
            } else if ball_x >= ARENA_WIDTH - ball.radius {
                update_score(
                    &mut scores.p1_score,
                    ui_text.get_mut(score_text.p1_score_text),
                );
                true
            } else {
                false
            };

            // Reset the ball and reverse it's direction
            if did_hit {
                ball.velocity[0] = -ball.velocity[0];
                transform.set_translation_x(ARENA_WIDTH / 2.0);
                transform.set_translation_y(ARENA_HEIGHT / 2.0);
                println!("Score: | {:^3} | {:^3} |", scores.p1_score, scores.p2_score);
            }
        }
    }
}
