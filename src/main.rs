//! Pong Tutorial 5

mod audio;
mod pong;
mod systems;

use amethyst::{
    audio::AudioBundle,
    core::transform::TransformBundle,
    input::{InputBundle, StringBindings},
    prelude::*,
    renderer::{
        plugins::{RenderFlat2D, RenderToWindow},
        types::DefaultBackend,
        RenderingBundle,
    },
    ui::{RenderUi, UiBundle},
    utils::application_root_dir,
};

use crate::pong::Pong;

fn main() -> amethyst::Result<()> {
    amethyst::start_logger(Default::default());
    let app_root = application_root_dir()?;
    let display_config_path = app_root.join("config").join("display.ron");
    let binding_path = app_root.join("config").join("bindings.ron");

    let input_bundle =
        InputBundle::<StringBindings>::new().with_bindings_from_file(binding_path)?;

    mod sysname {
        pub const PADDLES: &'static str = "paddle_system";
        pub const BALLS: &'static str = "move_balls_system";
        pub const BOUNCE_BALLS: &'static str = "bounce_system";
        pub const INPUT: &'static str = "input_system";
        pub const WINNER: &'static str = "winner_system";
    }
    let game_data = GameDataBuilder::default()
        .with_bundle(
            RenderingBundle::<DefaultBackend>::new()
                // The RenderToWindow plugin provides all the scaffolding for opening a window and drawing on it
                .with_plugin(
                    RenderToWindow::from_config_path(display_config_path)?
                        .with_clear([0.0, 0.0, 0.0, 1.0]),
                )
                // RenderFlat2D plugin is used to render entities with a `SpriteRender` component.
                .with_plugin(RenderFlat2D::default())
                .with_plugin(RenderUi::default()),
        )?
        .with_bundle(TransformBundle::new())?
        .with_bundle(input_bundle)?
        .with_bundle(UiBundle::<StringBindings>::new())?
        .with_bundle(AudioBundle::default())?
        .with(systems::PaddleSystem, sysname::PADDLES, &[sysname::INPUT])
        .with(systems::MoveBallsSystem, sysname::BALLS, &[])
        .with(
            systems::BounceSystem,
            sysname::BOUNCE_BALLS,
            &[sysname::PADDLES, sysname::BALLS],
        )
        .with(systems::WinnerSystem, sysname::WINNER, &[sysname::BALLS]);

    let assets_dir = app_root.join("assets");
    let mut game = Application::new(assets_dir, Pong::default(), game_data)?;
    game.run();
    Ok(())
}
