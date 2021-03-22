use amethyst::{
    assets::{AssetStorage, Handle, Loader},
    core::transform::{Transform, TransformBundle},
    ecs::{Component, DenseVecStorage},
    prelude::*,
    renderer::{
        plugins::{RenderFlat2D, RenderToWindow},
        types::DefaultBackend,
        RenderingBundle,
    },
    renderer::{Camera, ImageFormat, SpriteRender, SpriteSheet, SpriteSheetFormat, Texture},
    utils::application_root_dir,
};

pub const ARENA_HEIGHT: f32 = 100.0;
pub const ARENA_WIDTH: f32 = 100.0;

pub const PADDLE_HEIGHT: f32 = 16.0;
pub const PADDLE_WIDTH: f32 = 4.0;

pub const BALL_VELOCITY_X: f32 = 75.0;
pub const BALL_VELOCITY_Y: f32 = 75.0;
pub const BALL_RADIUS: f32 = 2.0;

#[derive(PartialEq, Eq)]
pub enum Side {
    Left,
    Right,
}

pub struct Paddle {
    pub side: Side,
    pub width: f32,
    pub height: f32,
}

impl Paddle {
    fn new(side: Side) -> Paddle {
        Paddle {
            side,
            width: PADDLE_WIDTH,
            height: PADDLE_HEIGHT,
        }
    }
}

impl Component for Paddle {
    type Storage = DenseVecStorage<Self>;
}

pub struct Ball {
    pub velocity: [f32; 2],
    pub radius: f32,
}

impl Ball {
    fn new() -> Ball {
        Ball {
            velocity: [BALL_VELOCITY_X, BALL_VELOCITY_Y],
            radius: BALL_RADIUS,
        }
    }
}

impl Component for Ball {
    type Storage = DenseVecStorage<Self>;
}

pub struct Pong;
impl SimpleState for Pong {
    fn on_start(&mut self, data: StateData<'_, GameData<'_, '_>>) {
        let world = data.world;

        // load the sprite sheet necessary for the graphics
        let sprite_sheet_handle = load_sprite_sheet(world);

        // N.B. The following line isn't needed since there's a system using the Paddle compononet.
        // Otherwise, we'd need it.
        // world.register::<Paddle>();

        initialise_camera(world);
        initialise_paddles(world, sprite_sheet_handle.clone());
        initialise_ball(world, sprite_sheet_handle);
    }
}

fn initialise_camera(world: &mut World) {
    let mut transform = Transform::default();
    transform.set_translation_xyz(ARENA_WIDTH * 0.5, ARENA_HEIGHT * 0.5, 1.0);
    world
        .create_entity()
        .with(Camera::standard_2d(ARENA_WIDTH, ARENA_HEIGHT))
        .with(transform)
        .build();
}

fn initialise_paddles(world: &mut World, sprite_sheet_handle: Handle<SpriteSheet>) {
    let sprite_render = SpriteRender::new(sprite_sheet_handle, 0);

    let mut left_transform = Transform::default();
    let mut right_transform = Transform::default();

    let mid_arena_y = ARENA_HEIGHT / 2.0;
    let mid_paddle_x = PADDLE_WIDTH / 2.0;
    left_transform.set_translation_xyz(mid_paddle_x, mid_arena_y, 0.0);
    right_transform.set_translation_xyz(ARENA_WIDTH - mid_paddle_x, mid_arena_y, 0.0);

    world
        .create_entity()
        .with(Paddle::new(Side::Left))
        .with(sprite_render.clone())
        .with(left_transform)
        .build();

    world
        .create_entity()
        .with(sprite_render.clone())
        .with(Paddle::new(Side::Right))
        .with(right_transform)
        .build();
}

fn initialise_ball(world: &mut World, sprite_sheet_handle: Handle<SpriteSheet>) {
    let sprite_render = SpriteRender::new(sprite_sheet_handle, 1);

    let mut transform = Transform::default();

    transform.set_translation_xyz(ARENA_WIDTH / 2.0, ARENA_HEIGHT / 2.0, 0.0);

    world
        .create_entity()
        .with(Ball::new())
        .with(sprite_render.clone())
        .with(transform)
        .build();
}

fn load_sprite_sheet(world: &mut World) -> Handle<SpriteSheet> {
    // Load hte sprite sheet necessary to render the graphics.
    // The texture is the pixel data
    // `texture_handle` is a cloneable reference to the texture
    let loader = world.read_resource::<Loader>();

    let texture_handle = {
        let texture_storage = world.read_resource::<AssetStorage<Texture>>();
        loader.load(
            "texture/pong_spritesheet.png",
            ImageFormat::default(),
            (),
            &texture_storage,
        )
    };

    let sprite_sheet_store = world.read_resource::<AssetStorage<SpriteSheet>>();
    loader.load(
        "texture/pong_spritesheet.ron",
        SpriteSheetFormat(texture_handle),
        (),
        &sprite_sheet_store,
    )
}
