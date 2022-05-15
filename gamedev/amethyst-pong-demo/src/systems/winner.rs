use amethyst::{
    assets::AssetStorage,
    audio::output::Output,
    audio::Source,
    core::Transform,
    derive::SystemDesc,
    ecs::{Join, Read, ReadExpect, System, SystemData, Write, WriteStorage},
    ui::UiText,
};

use crate::audio::{play_score_sound, Sounds};
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
        Read<'s, AssetStorage<Source>>,
        ReadExpect<'s, Sounds>,
        Option<Read<'s, Output>>,
    );

    fn run(&mut self, system_data: Self::SystemData) {
        let (
            mut balls,
            mut transforms,
            mut ui_text,
            mut scores,
            score_text,
            storage,
            sounds,
            audio_output,
        ) = system_data;
        for (ball, transform) in (&mut balls, &mut transforms).join() {
            fn update_score(score: &mut i32, ui_entity: Option<&mut UiText>) {
                *score = (*score + 1).min(999);
                if let Some(text) = ui_entity {
                    text.text = score.to_string();
                }
            }

            let ball_x = transform.translation().x;

            let maybe_score_data = if ball_x <= ball.radius {
                Some((&mut scores.p2_score, score_text.p2_score_text))
            } else if ball_x >= ARENA_WIDTH - ball.radius {
                Some((&mut scores.p1_score, score_text.p1_score_text))
            } else {
                None
            };

            if let Some(score_data) = maybe_score_data {
                update_score(score_data.0, ui_text.get_mut(score_data.1));
                play_score_sound(&*sounds, &storage, audio_output.as_deref());

                // Reset the ball and reverse it's direction
                ball.velocity[0] = -ball.velocity[0];
                transform.set_translation_x(ARENA_WIDTH / 2.0);
                transform.set_translation_y(ARENA_HEIGHT / 2.0);
                // println!("Score: | {:^3} | {:^3} |", scores.p1_score, scores.p2_score);
            }
        }
    }
}
