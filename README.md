# Comfy Anims
`comfy_anims` is a small library that makes it easy to create beautiful, smooth animations in the Pokémon decomp projects.  In general, implementing nice animations requires effort, so people tend to forgo it (e.g. menu cursors that move instantaneously, or windows that enter the screen linearly).  With `comfy_anims`, adding [juice](https://www.youtube.com/watch?v=Fy0aCDmgnxg) to the UI is a breeze.

**Table of Contents**
- [When to use Comfy Anims?](#when-to-use-comfy-anims)
- [Setup](#setup)
- [Concepts Overview](#concepts-overview)
  * [Spring](#spring)
  * [Easing](#easing)
- [Usage & API](#usage--api)
  * [Configs](#configs)
    + [Spring Config](#spring-config)
    + [Easing Config](#easing-config)
  * [Running an Animation](#running-an-animation)
- [Tips & Tricks](#tips--tricks)

## When to use Comfy Anims?
If you've never worked with animations before, it can be hard to pinpoint when to use them.  And sometimes, it's hard to notice *great* animations, because they feel natural within the UI.  To get your imagination going, here are a few examples from the base game that have been enhanced by `comfy_anims`.
1. The PokéNav menu UI entrance is linear, which feels rigid.  With `comfy_anims`, we can add some springy flair to the entrance:

    | Before  | After |
    | ------------- | ------------- |
    | ![vanilla_pokenav](https://github.com/user-attachments/assets/8865e7d5-b505-45e1-90c0-f36ea8be209a) | ![comfy_pokenav](https://github.com/user-attachments/assets/0fedeca7-1bfc-450d-bf6e-f06a8849bf6d) |

2. The start menu's cursor moves instantly between menu options.  We can make the cursor movement smooth with `comfy_anims`:

    | Before  | After |
    | ------------- | ------------- |
    | ![vanilla_cursor](https://github.com/user-attachments/assets/d5ff31bc-5b75-467f-b7bb-af6a5b0ab9aa) | ![comfy_cursor](https://github.com/user-attachments/assets/9095092e-887e-4db3-8219-786b9947886c) |

3. The player Pokémon's health bar window enters linearly from the right, which feels rigid.  To make it snappy and comfortably settle into its resting place, we can use `comfy_anims` easing to smoothly transition its `x` coordinate:

    | Before  | After |
    | ------------- | ------------- |
    | ![vanilla_healthbar](https://github.com/user-attachments/assets/955760e2-7a66-41dd-9066-5384dd07a1a9) | ![comfy_healthbar](https://github.com/user-attachments/assets/9da91ee4-d175-4e44-b6da-b9238bf0d663) |

## Setup
Adding `comfy_anims` to your decomp project is straightforward.

Simply copy-paste [`comfy_anim.c`](https://github.com/huderlem/pokeemerald/blob/comfy_anims/comfy_anim.c) and [`comfy_anim.h`](https://github.com/huderlem/pokeemerald/blob/comfy_anims/comfy_anim.h) into your decomp project's `src/` and `include/` folder, respectively. For example, if you are working with `pokeemerald`, you should add the two files here:
- `pokeemerald/src/comfy_anim.c`
- `pokeemerald/include/comfy_anim.h`

At this point, it's a good idea to compile your project with `make` to ensure the game builds successfully after including those two `comfy_anim` files.

## Concepts Overview
The `comfy_anims` library exposes the concept of an animated value, which we'll call a *ComfyAnim*.  A ComfyAnim can be one of two kinds: ***spring*** or ***easing***.  These are similar, but they have different use cases.  Let's learn more about these two kinds of ComfyAnim values now.

### Spring
A *spring* ComfyAnim updates its value based on spring physics.  It has properties like *mass*, *tension*, and *friction* that all determine how the value updates over time.  For example, imagine you have a small golf ball attached to a high-tension spring.  When the spring is stretched and released, the golf ball will move extremely quickly to the end of start of the spring.  Using a heavy rock would move much slower than the golf ball because it has larger *mass*.

To continue the analogy, when the golf ball reaches the start of the spring, it will wobble back and forth until friction eventually causes it to come to a complete stop.

When using a ComfyAnim *spring*, you don't care or know exactly how long the animation will take because it's based on those physical properties.  You might even want to update the spring's target value while the animation is ongoing!

In the following GIF, 4 separate spring ComfyAnim values are used to move the square around the screen. One each for the `x` and `y` coordinates, `scale` and `rotation` of the square.

![comfy_square_chase](https://github.com/user-attachments/assets/2ad53394-8c2e-41e9-9999-744bb8d56052)

### Easing
An *easing* ComfyAnim updates its value based on an *easing function*.  It also has a **fixed duration**.  Use an *easing* ComfyAnim value when you know exactly how long the animation should take, and you have a desired easing function to apply to the value.

To explain what an *easing function* is, imagine a car braking to stop at a stoplight.  The car doesn't stop immediately, rather, it gradually slows down.  Its position on the road can be represented with an "ease out" function because it starts fast and slows down over time while it's braking.

This concept is often very useful in UI animations.  For example, you could use an "ease out" function to make a window slide in from outside of the screen and gently settle into its final resting position (similar to a car braking).  Or you could use an "ease ***in***" function to make a window exit the screen where it starts out slowly to give the illusion of weight.

This website, https://easings.net/#, provides a ton of useful visualizations and explanations of various easing functions.  The common and most-useful easing functions are available in `comfy_anims`.

In the following GIF, 4 separate easing ComfyAnim values are used to move the square around. One each for the `x` and `y` coordinates, `scale` and `rotation` of the square.

![comfy_square](https://github.com/user-attachments/assets/f73336a6-e41d-4c65-af73-b8f0a54b545a)

## Usage & API
Assuming you read the sections above, you should be familiar with *spring* and *easing*--the two kinds of ComfyAnim values.  First, we'll go over their configurations, and then we'll go through some real examples of how to use them.

### Configs
Both types of ComfyAnim values must be configured to control their behavior.

#### Spring Config
A spring ComfyAnim is configured with `struct ComfyAnimSpringConfig`.  Its members are:
- `s32 from` - The initial value of the ComfyAnim.  __**`Q_24_8` fixed-point value**__
- `s32 to` - The target/destination value of the ComfyAnim.  __**`Q_24_8` fixed-point value**__
- `s32 tension` - The tension of the spring.      - __**`Q_24_8` fixed-point value**__
- `s32 friction` - The friction of the spring.      - __**`Q_24_8` fixed-point value**__
- `s32 mass` - The mass of the conceptual object attached to the spring.      - __**`Q_24_8` fixed-point value**__
- `u32 clampAfter` - The number of times the ComfyAnim value can reach the `to` value before ending the animation.
    - If `clampAfter` is `0`, that means that the spring value will wobble around the `to` value until it eventually comes to a complete stop due to friction.  `clampAfter=1` would mean the spring animation ends the very first time it reached the target value.  `clampAfter=2` means the spring overshoots once, and so on.
- `u32 delayFrames` - The number of frames to delay the start of the animation.
    - For example, `60` would result in the spring value being stuck at its original `from` position for 60 frames, after which the spring animation would continue normally.

To create a spring config, call `InitComfyAnimConfig_Spring()`.
```c
// First, initialize the config struct with defaults.
struct ComfyAnimSpringConfig config;
InitComfyAnimConfig_Spring(&config);
// Then, set the desired values. Here, we are animating the value from 0 -> 100.
config.from = Q_24_8(0);
config.to = Q_24_8(100);
config.mass = Q_24_8(100);
config.tension = Q_24_8(185);
config.friction = Q_24_8(900);
```

#### Easing Config
An easing ComfyAnim is configured with `struct ComfyAnimEasingConfig`.  Its members are:
- `s32 from` - The initial value of the ComfyAnim.  __**`Q_24_8` fixed-point value**__
- `s32 to` - The target/destination value of the ComfyAnim.  __**`Q_24_8` fixed-point value**__
- `u32 durationFrames` - The total duration of the animation, in frames. (The Game Boy Advance runs at 60 frames per second).
- `ComfyAnimEasingFunc easingFunc` - The easing function used to advance the ComfyAnim value.  There are a bunch of easing functions provided by `comfy_anims`, but you can also write your own if needed:
    - `ComfyAnimEasing_EaseInQuad`
    - `ComfyAnimEasing_EaseOutQuad`
    - `ComfyAnimEasing_EaseInOutQuad`
    - `ComfyAnimEasing_EaseInCubic`
    - `ComfyAnimEasing_EaseOutCubic`
    - `ComfyAnimEasing_EaseInOutCubic`
    - `ComfyAnimEasing_EaseInOutBack`
    - `ComfyAnimEasing_Linear`
- `u32 delayFrames` - The number of frames to delay the start of the animation.
    - For example, `60` would result in the spring value being stuck at its original `from` position for 60 frames, after which the spring animation would continue normally.

To create an easing config, call `InitComfyAnimConfig_Easing()`.
```c
// First, initialize the config struct with defaults.
struct ComfyAnimEasingConfig config;
InitComfyAnimConfig_Easing(&config);
// Then, set the desired values. Here, we are animating the value from 64 -> 8 over
// the course of 2 seconds (2 seconds = 120 frames).
config.from = Q_24_8(64);
config.to = Q_24_8(8);
config.durationFrames = 120;
config.easingFunc = ComfyAnimEasing_EaseInOutCubic;
```

To help visualize the behavior of the easing functions, see the below animated GIFs, in which an easing ComfyAnim is used to control the `x` coordinate of a sprite:

| Easing Function | Demo GIF |
|--|--|
|`ComfyAnimEasing_Linear` | ![comfy_easelineargif](https://github.com/user-attachments/assets/f3244e3d-06a9-42d3-ad4a-3ef05cb76907) |
|`ComfyAnimEasing_EaseInQuad` | ![comfy_easeinquad](https://github.com/user-attachments/assets/a57880b8-7992-44fb-8b90-3d30d4580005) |
|`ComfyAnimEasing_EaseOutQuad` | ![comfy_easeoutquad](https://github.com/user-attachments/assets/5253dcf0-5243-468b-b79c-c74e6bf54274) |
|`ComfyAnimEasing_EaseInOutQuad` | ![comfy_easeinoutquad](https://github.com/user-attachments/assets/eed92e78-092b-45b1-86f6-9efedce22a32) |
|`ComfyAnimEasing_EaseInCubic` | ![comfy_easeincubic](https://github.com/user-attachments/assets/012289bc-7970-4432-a9f8-717f69557ad7) |
|`ComfyAnimEasing_EaseOutCubic` | ![comfy_easeoutcubic](https://github.com/user-attachments/assets/1f66792e-03a5-4448-9fef-4b7cb116a7c5) |
|`ComfyAnimEasing_EaseInOutCubic` | ![comfy_easeinoutcubic](https://github.com/user-attachments/assets/0bf2e8d6-0034-4569-add7-420e01045c56) |
|`ComfyAnimEasing_EaseInOutBack` | ![comfy_easeinoutback](https://github.com/user-attachments/assets/45714378-9ccc-4634-aba3-635ecb7c406b) |

### Running an Animation
Now that we know how to configure a ComfyAnim, let's look at how to actually run them so their values update over time.

To begin, you must create a `struct ComfyAnim` from the config object.  There are two ways to do this, and which one you use depends on your use case.

1. The first option is to call `CreateComfyAnim_Easing()` or `CreateComfyAnim_Spring()`.  This will take your config and create a `struct ComfyAnim` in the global `gComfyAnims` array.  This is the same way Tasks work when calling `CreateTask()` (i.e. `gTasks`).   It returns a `u8 animId`, which can then be used to index into `gComfyAnims` in order to access the in-progress animation.  Similar to `RunTasks()`, we need to call `AdvanceComfyAnimations()` once per frame so that the animations update--I recommend calling this inside the  relevant `MainCallback`, right after `RunTasks()`.

2. The second option is to manually maintain the `struct ComfyAnim`, rather than having it exist in the `gComfyAnims` global array.  Call `InitComfyAnim_Easing()` or `InitComfyAnim_Spring()` to initialize your `struct ComfyAnim` object.  To update this ComfyAnim object, call `TryAdvanceComfyAnim()` once per frame--typically, you'd do this in a sprite callback or Task.

Now that the animation is created and updating every frame, we can read its current value and do something with it!  To read the animation's current value, call `ReadComfyAnimValueSmooth()`.  After reading the value, check if the animation has completed.  If you're using method 1 from above (i.e. making use of `gComfyAnims`), then you need to make sure to *release* the animation when it's completed--just like how you would call `DestroyTask` to complete a task.  Similarly, when leaving the current screen/context, you'll want to release all of the comfy anims by calling `ReleaseComfyAnims()`--do this everywhere you'd call `ResetTasks()`.

Now let's put it all together.  For this example, we'll animate a sprite's `x` coordinate to move it across the screen.  Let's look at how to achieve this with both methods described above:

**Method 1**
```c
void MainCallback(void)
{
    ...
    RunTasks();
    // Process all of the active animations in gComfyAnims.
    AdvanceComfyAnimations();
    ...
}

void ExitScreen(void)
{
    ...
    ResetTasks();
    // Release all the animations when leaving this screen.
    ReleaseComfyAnims();
    ...
}

void StartAnimation(void)
{
    u8 animId, spriteId;
    struct ComfyAnimEasingConfig config;

    // Setup config for easing animation.
    InitComfyAnimConfig_Easing(&config);
    config.durationFrames = 60;
    config.from = Q_24_8(0);
    config.to = Q_24_8(128);
    config.easingFunc = ComfyAnimEasing_EaseOutCubic;

    // Create sprite for animation.
    spriteId = CreateSprite(spriteTemplate, ...);
    gSprites[spriteId].callback = UpdateSprite;

    // Start animation and save the animId in sprite's data[0].
    gSprites[spriteId].data[0] = CreateComfyAnim_Easing(&config);
}

void UpdateSprite(struct Sprite *sprite)
{
    int animId = sprite->data[0];
    sprite->x = ReadComfyAnimValueSmooth(&gComfyAnims[animId]);
    if (gComfyAnims[animId].completed)
    {
        ReleaseComfyAnim(animId);
        sprite->callback = SpriteCallbackDummy;
    }
}
```

**Method 2**
```c
// We have a ComfyAnim struct in EWRAM--we don't want to rely on gComfyAnims.
EWRAM_DATA struct ComfyAnim myAnim= {0};

void StartAnimation(void)
{
    u8 spriteId;
    struct ComfyAnimEasingConfig config;

    // Setup config for easing animation.
    InitComfyAnimConfig_Easing(&config);
    config.durationFrames = 60;
    config.from = Q_24_8(0);
    config.to = Q_24_8(128);
    config.easingFunc = ComfyAnimEasing_EaseOutCubic;

    // Create sprite for animation.
    spriteId = CreateSprite(spriteTemplate, ...);
    gSprites[spriteId].callback = UpdateSprite;

    // Start animation directly with the anim object.
    InitComfyAnim_Easing(&config, &myAnim);
}

void UpdateSprite(struct Sprite *sprite)
{
    // Manually update the animation.
    TryAdvanceComfyAnim(&myAnim);
    sprite->x = ReadComfyAnimValueSmooth(&myAnim);
    if (myAnim.completed)
    {
        sprite->callback = SpriteCallbackDummy;
    }
}
```

## Tips & Tricks
- Multiple ComfyAnims can run at the same time.  For example, to move a sprite diagonally across the screen, you would need to create one for the `x` coordinate and another for the `y` coordinate.
- Springs can be used to "chase" a value indefinitely.  For example, a menu cursor's position could be represented by a spring ComfyAnim that never completes.  Whenever the player moves the cursor, you can update the spring ComfyAnim's `.to` value appropriately, and it will naturally start animating accordingly.  This concept could also be used for something like a floating Gastly following behind the player while walking around.
- **DO NOT FORGET to use `Q_24_8()` when setting the CONFIG VALUES!!!!**  It's very easy to forget this.
- Springs are very powerful, however, they often require a bunch of tweaking `tension`, `mass` and `friction` until it feels *juuust* right.  I would recommend playing around with various values to get a sense of how it affects the spring's behavior.
- To add a "wind up" effect to an animation, try chaining two springs together.  The first spring is a small movement backwards, and when it completes, start the second spring to animate to the final position.
  
