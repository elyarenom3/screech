#pragma once

#include <Arduino.h>

// ======================================================================
// --- WEB INTERFACE HTML ---
// ======================================================================

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Screech Access Point Controller</title>
  <meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">
  <meta name="theme-color" content="#15150e">
  <meta charset="UTF-8">
  <style>
    :root {
      --content-color: #00aaff;
      --content-color-dark: #0091e0;
      --content-color-darker: #0077bb;
      --content-color-glow: rgba(0, 170, 255, 0.35);
      --red: #ef4b2a;
      --red-dark: #7d1e0f;
      --mustard: #f2a33a;
      --cream: #e8e6d0;
      --cream-dark: #d0cfb5;
      --plastic-edge: #86836a;
      --ink: #23221a;
      --lcd: #b4bf9b;
    }

    * {
      box-sizing: border-box;
      user-select: none;
      -webkit-user-select: none;
      -webkit-touch-callout: none;
      -webkit-tap-highlight-color: transparent;
    }
    input, select {
      user-select: text;
      -webkit-user-select: text;
    }

    body {
      font-family: 'Helvetica Neue', Arial, 'Segoe UI', Roboto, sans-serif;
      text-align: center;
      color: #efe5d0;
      touch-action: manipulation;
      margin: 0;
      padding: 10px 10px 30px;
      overflow-x: hidden;
      min-height: 100vh;
      background:
        radial-gradient(ellipse 90% 60% at 50% 8%, #6b6a4f 0%, #43432f 42%, transparent 100%),
        linear-gradient(180deg, #2f2f22 0%, #15150e 100%);
      background-attachment: fixed;
    }
    /* faint scan lines, like a printed catalogue or a CRT */
    body::after {
      content: '';
      position: fixed;
      inset: 0;
      pointer-events: none;
      z-index: 300;
      background: repeating-linear-gradient(0deg, rgba(0, 0, 0, 0.07) 0 1px, transparent 1px 3px);
    }

    /* ---------- Hero ---------- */
    .hero { padding-top: 14px; }

    .logo {
      margin: 0;
      line-height: 1;
    }
    .logo small {
      display: block;
      font-size: 12px;
      font-weight: 700;
      letter-spacing: 0.45em;
      padding-left: 0.45em;
      color: var(--mustard);
      margin-bottom: 6px;
    }
    .logo span {
      display: inline-block;
      font-family: 'Arial Black', 'Helvetica Neue', Impact, Arial, sans-serif;
      font-weight: 900;
      font-size: 54px;
      letter-spacing: 0.04em;
      color: var(--red);
      text-shadow: 3px 3px 0 #1c100a, 1px 1px 0 #1c100a, -1px -1px 0 #f7d9b0;
    }

    .logo small i,
    .maker i {
      font-style: normal;
      color: var(--red);
    }
    .maker i {
      color: var(--mustard);
      font-size: 16px;
      margin-right: 4px;
    }

    .stage {
      position: relative;
      width: 240px;
      height: 176px;
      margin: 14px auto 2px;
    }
    .vtext {
      position: absolute;
      left: 0;
      top: 12px;
      writing-mode: vertical-rl;
      white-space: nowrap;
      font-size: 10px;
      font-weight: 700;
      letter-spacing: 0.1em;
      color: #e6e3c8;
      opacity: 0.85;
      padding-right: 8px;
      border-right: 1px solid rgba(230, 227, 200, 0.45);
    }
    .bot {
      position: absolute;
      left: 45px;
      top: 50px;
      width: 160px;
    }
    .head {
      position: relative;
      height: 78px;
      border-radius: 42px 42px 28px 28px / 36px 36px 26px 26px;
      background: linear-gradient(180deg, #faf6ea 0%, #e4dcc6 60%, #cfc5ab 100%);
      border: 1px solid #a79c84;
      box-shadow:
        inset 4px 4px 8px rgba(255, 255, 255, 0.8),
        inset -6px -6px 12px rgba(0, 0, 0, 0.16),
        0 10px 22px rgba(0, 0, 0, 0.55);
    }
    .visor {
      position: absolute;
      left: 11px;
      right: 11px;
      top: 11px;
      bottom: 11px;
      border-radius: 32px;
      background: linear-gradient(180deg, #2a2019, #0f0a07);
      display: flex;
      align-items: center;
      justify-content: center;
      gap: 12px;
      box-shadow: inset 0 3px 8px rgba(0, 0, 0, 0.8);
    }
    .lens {
      width: 46px;
      height: 46px;
      border-radius: 50%;
      border: 4px solid #3a2e26;
      background: radial-gradient(circle at 50% 45%, #ffffff 0 7%, var(--content-color) 36%, var(--content-color-darker) 74%, #0a0705 100%);
      box-shadow: 0 0 22px var(--content-color-glow), inset 0 0 10px rgba(0, 0, 0, 0.55);
      animation: lamp 3.4s ease-in-out infinite;
    }
    .neck {
      width: 52px;
      height: 22px;
      margin: -2px auto 0;
      border-radius: 4px;
      background: repeating-linear-gradient(180deg, #14100d 0 3px, #4a3f36 3px 5px);
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.5);
    }
    .antenna {
      position: absolute;
      right: 20px;
      bottom: 100%;
      width: 2px;
      height: 46px;
      background: linear-gradient(180deg, #efe5d0, #9a8e78);
    }
    .antenna::after {
      content: '';
      position: absolute;
      top: -4px;
      left: -2px;
      width: 6px;
      height: 6px;
      border-radius: 50%;
      background: #efe5d0;
    }

    /* Command Queue Status (small LCD) */
    .command-queue,
    .gamepad-status {
      display: inline-block;
      font-family: 'Courier New', ui-monospace, Menlo, monospace;
      font-weight: 700;
      font-size: 13px;
      letter-spacing: 0.12em;
      text-transform: uppercase;
      color: #25301d;
      background: linear-gradient(180deg, #c6cfb0, var(--lcd));
      border: 3px solid #cfc6ae;
      border-radius: 6px;
      padding: 5px 12px;
      box-shadow: inset 0 2px 6px rgba(0, 0, 0, 0.4), 0 2px 0 rgba(0, 0, 0, 0.35);
      transition: all 0.15s;
    }
    .command-queue { margin: 6px 0 14px; }
    .command-queue.full {
      color: #8a1b0c;
      background: linear-gradient(180deg, #e2c2b0, #cfa48f);
    }

    /* 80s appliance stripes */
    .stripes {
      height: 16px;
      width: min(100% - 20px, 920px);
      margin: 0 auto 22px;
      border-radius: 2px;
      background: linear-gradient(180deg,
        var(--red) 0 4px, transparent 4px 6px,
        #e6e3c8 6px 10px, transparent 10px 12px,
        var(--content-color) 12px 16px);
      clip-path: polygon(0 0, 100% 0, calc(100% - 12px) 100%, 12px 100%);
    }

    /* ---------- Sections (cream plastic panels) ---------- */
    .sections-container {
      display: flex;
      flex-direction: column;
      gap: 18px;
      max-width: 1400px;
      margin: 0 auto;
    }
    .section-column {
      display: flex;
      flex-direction: column;
      gap: 18px;
    }

    .section {
      position: relative;
      color: var(--ink);
      background: linear-gradient(180deg, var(--cream), var(--cream-dark));
      border: 1px solid #f7f1e1;
      border-radius: 14px;
      padding: 22px 16px 18px;
      margin: 0 auto;
      width: calc(100% - 20px);
      max-width: 450px;
      box-shadow:
        inset 0 2px 0 rgba(255, 255, 255, 0.75),
        inset 0 -4px 0 rgba(0, 0, 0, 0.12),
        0 14px 30px rgba(0, 0, 0, 0.55);
    }
    /* corner screws */
    .section::before {
      content: '';
      position: absolute;
      inset: 7px;
      pointer-events: none;
      background:
        radial-gradient(circle, #8f846d 0 2.5px, transparent 3px) 0 0 / 8px 8px no-repeat,
        radial-gradient(circle, #8f846d 0 2.5px, transparent 3px) 100% 0 / 8px 8px no-repeat,
        radial-gradient(circle, #8f846d 0 2.5px, transparent 3px) 0 100% / 8px 8px no-repeat,
        radial-gradient(circle, #8f846d 0 2.5px, transparent 3px) 100% 100% / 8px 8px no-repeat;
    }

    .section-title {
      display: flex;
      align-items: center;
      gap: 10px;
      margin: 0 0 18px 0;
      font-size: 12px;
      font-weight: 800;
      letter-spacing: 0.2em;
      text-transform: uppercase;
      color: #2f2d1f;
      white-space: nowrap;
    }
    .section-title::before,
    .section-title::after {
      content: '';
      flex: 1;
      height: 4px;
      border-top: 1px solid var(--plastic-edge);
      border-bottom: 1px solid var(--plastic-edge);
    }
    .section-title small {
      font-size: 11px;
      letter-spacing: 0.1em;
      color: #6b6850;
    }

    /* ---------- Keys ---------- */
    button {
      font-family: inherit;
      background: linear-gradient(180deg, #dcdcd5, #aeaea6);
      border: 1px solid #6d6c66;
      color: var(--ink);
      padding: 15px;
      font-size: 17px;
      border-radius: 10px;
      cursor: pointer;
      box-shadow: 0 5px 0 #5f5e58, 0 9px 12px rgba(0, 0, 0, 0.3), inset 0 2px 0 rgba(255, 255, 255, 0.6);
      transition: transform 0.07s, box-shadow 0.07s, filter 0.15s;
      font-weight: 800;
    }
    button:active {
      transform: translateY(4px);
      box-shadow: 0 1px 0 #5f5e58, 0 2px 5px rgba(0, 0, 0, 0.3), inset 0 2px 0 rgba(255, 255, 255, 0.4);
    }
    @media (hover: hover) {
      button:hover { filter: brightness(1.06); }
    }

    /* D-Pad Controls */
    .dpad-container {
      display: flex;
      flex-direction: column;
      align-items: center;
      gap: 20px;
      width: 100%;
    }
    .dpad {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      grid-template-rows: repeat(2, 1fr);
      gap: 12px;
      width: 100%;
      max-width: 294px;
      aspect-ratio: 3 / 2;
    }
    .dpad button {
      font-size: 30px;
      width: 100%;
      height: 100%;
      min-height: 70px;
      padding: 0;
    }
    .dpad button:active {
      color: var(--content-color-darker);
      background: linear-gradient(180deg, #c3c3bb, #9e9e96);
    }
    .spacer {
      visibility: hidden;
    }

    /* Pose Grid (cassette-deck style keys) */
    .grid {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      gap: 12px 10px;
    }
    .btn-pose {
      background: linear-gradient(180deg, #fbf8ee, #e2d9c1);
      border: 1px solid #9b9078;
      border-top: 5px solid var(--content-color);
      border-radius: 8px;
      box-shadow: 0 4px 0 var(--plastic-edge), 0 8px 12px rgba(0, 0, 0, 0.25);
      color: var(--ink);
      font-size: 12px;
      letter-spacing: 0.1em;
      text-transform: uppercase;
      padding: 12px 2px 11px;
    }
    .btn-pose:active {
      background: linear-gradient(180deg, #e2d9c1, #cfc5aa);
      box-shadow: 0 1px 0 var(--plastic-edge), 0 2px 5px rgba(0, 0, 0, 0.25);
    }

    /* Special Buttons */
    .btn-stop-all {
      display: flex;
      align-items: center;
      justify-content: center;
      gap: 12px;
      width: 100%;
      background: linear-gradient(180deg, #f35e3f, #c8321a);
      border: 1px solid var(--red-dark);
      box-shadow: 0 5px 0 var(--red-dark), 0 10px 16px rgba(0, 0, 0, 0.35), inset 0 2px 0 rgba(255, 255, 255, 0.35);
      color: #fff;
      text-shadow: 0 1px 2px rgba(0, 0, 0, 0.4);
      font-size: 18px;
      letter-spacing: 0.22em;
      padding: 15px;
    }
    .btn-stop-all small {
      font-size: 13px;
      letter-spacing: 0.15em;
      font-weight: 700;
      opacity: 0.92;
    }
    .btn-stop-all:active {
      box-shadow: 0 1px 0 var(--red-dark), 0 2px 6px rgba(0, 0, 0, 0.35), inset 0 2px 0 rgba(255, 255, 255, 0.25);
    }

    .btn-settings {
      padding: 12px 25px;
      font-size: 15px;
      letter-spacing: 0.08em;
    }
    .btn-settings small {
      font-size: 12px;
      margin-left: 8px;
      color: #5a5844;
    }

    /* Motor Controls */
    .lock-indicator {
      font-size: 12px;
      font-weight: 700;
      color: #ff8a6e;
      text-align: center;
      margin-top: 5px;
      display: none;
    }
    .lock-indicator.active {
      display: block;
    }

    .motor-controls {
      margin-top: 10px;
    }
    .motor-slider {
      margin: 15px 0;
    }
    .motor-slider label {
      display: flex;
      justify-content: space-between;
      font-size: 12px;
      color: #dcdcc4;
      margin-bottom: 5px;
    }
    .motor-slider input[type="range"] {
      width: 100%;
      height: 8px;
      background: #45452f;
      border-radius: 5px;
      outline: none;
      -webkit-appearance: none;
      padding: 0;
      border: none;
    }
    .motor-slider input[type="range"]::-webkit-slider-thumb {
      -webkit-appearance: none;
      width: 24px;
      height: 24px;
      background: var(--cream);
      border: 4px solid var(--content-color);
      border-radius: 50%;
      cursor: pointer;
      box-shadow: 0 2px 6px rgba(0, 0, 0, 0.5);
    }
    .motor-slider input[type="range"]::-moz-range-thumb {
      width: 24px;
      height: 24px;
      background: var(--cream);
      border-radius: 50%;
      cursor: pointer;
      border: 4px solid var(--content-color);
      box-shadow: 0 2px 6px rgba(0, 0, 0, 0.5);
    }
    .motor-slider input[type="range"]:disabled {
      opacity: 0.45;
      cursor: not-allowed;
    }
    .motor-slider input[type="range"]:disabled::-webkit-slider-thumb,
    .motor-slider input[type="range"]:disabled::-moz-range-thumb {
      border-color: #777;
      cursor: not-allowed;
    }

    /* Gamepad Status (LCD with LED) */
    .gamepad-status::before {
      content: '';
      display: inline-block;
      width: 8px;
      height: 8px;
      border-radius: 50%;
      background: #6b705c;
      margin-right: 9px;
      vertical-align: 1px;
    }
    .gamepad-status.connected::before {
      background: #1fae4f;
      box-shadow: 0 0 8px #1fae4f;
    }

    /* ---------- Settings Panel (dark charcoal-brown) ---------- */
    .settings-panel {
      display: none;
      position: fixed;
      top: 0;
      left: 0;
      width: 100%;
      height: 100%;
      background: rgba(10, 10, 6, 0.9);
      z-index: 100;
      overflow-y: auto;
    }
    .settings-content {
      position: relative;
      background: linear-gradient(180deg, #30311f, #1f2016);
      border: 1px solid #5f5f46;
      max-width: 400px;
      margin: 30px auto;
      padding: 25px;
      border-radius: 14px;
      text-align: left;
      box-shadow: 0 14px 50px rgba(0, 0, 0, 0.7), inset 0 2px 0 rgba(255, 255, 255, 0.08);
    }
    .settings-content h3 {
      color: var(--cream);
      margin: 0 0 6px;
      text-align: center;
      font-size: 20px;
      letter-spacing: 0.2em;
      text-transform: uppercase;
    }
    .settings-content h3 small {
      font-size: 13px;
      letter-spacing: 0.15em;
      color: var(--mustard);
      margin-left: 8px;
    }
    .settings-section {
      margin: 20px 0;
      padding: 15px;
      background: rgba(0, 0, 0, 0.28);
      border: 1px solid #4b4b38;
      border-radius: 10px;
    }
    .settings-section h4 {
      color: var(--content-color);
      margin: 0 0 10px 0;
      font-size: 12px;
      text-transform: uppercase;
      letter-spacing: 0.2em;
    }
    .settings-content label {
      display: block;
      margin-top: 12px;
      font-weight: 700;
      color: #dcdcc4;
      font-size: 13px;
    }
    .settings-content input,
    .settings-content select {
      width: 100%;
      padding: 11px;
      margin-top: 5px;
      background: #10110a;
      color: #f4ecda;
      border: 1px solid #5f5f46;
      border-radius: 8px;
      box-sizing: border-box;
      font-size: 14px;
      font-family: inherit;
    }
    .settings-content input:focus,
    .settings-content select:focus {
      outline: none;
      border-color: var(--content-color);
      box-shadow: 0 0 0 3px var(--content-color-glow);
    }
    .btn-save {
      background: linear-gradient(180deg, var(--content-color), var(--content-color-dark));
      border: 1px solid var(--content-color-darker);
      box-shadow: 0 5px 0 var(--content-color-darker), 0 10px 16px rgba(0, 0, 0, 0.35), inset 0 2px 0 rgba(255, 255, 255, 0.35);
      text-shadow: 0 1px 2px rgba(0, 0, 0, 0.45);
      width: 100%;
      margin-top: 25px;
      color: #fff;
    }
    .btn-save:active {
      box-shadow: 0 1px 0 var(--content-color-darker), 0 2px 6px rgba(0, 0, 0, 0.35);
    }
    .btn-close {
      background: linear-gradient(180deg, #f35e3f, #c8321a);
      border: 1px solid var(--red-dark);
      box-shadow: 0 5px 0 var(--red-dark), 0 10px 16px rgba(0, 0, 0, 0.35), inset 0 2px 0 rgba(255, 255, 255, 0.35);
      text-shadow: 0 1px 2px rgba(0, 0, 0, 0.4);
      width: 100%;
      margin-top: 16px;
      color: #fff;
    }
    .btn-close:active {
      box-shadow: 0 1px 0 var(--red-dark), 0 2px 6px rgba(0, 0, 0, 0.35);
    }

    /* ---------- Footer badge ---------- */
    footer {
      display: flex;
      align-items: center;
      justify-content: space-between;
      gap: 12px;
      width: calc(100% - 20px);
      max-width: 450px;
      margin: 28px auto 0;
    }
    .series {
      border: 2px solid #e6e3c8;
      border-radius: 6px;
      padding: 5px 10px;
      font-size: 11px;
      font-weight: 800;
      letter-spacing: 0.1em;
      color: #e6e3c8;
      white-space: nowrap;
    }
    .series span {
      display: block;
      font-size: 8px;
      letter-spacing: 0.2em;
      font-weight: 700;
      border-top: 1px solid rgba(230, 227, 200, 0.5);
      margin-top: 2px;
      padding-top: 2px;
    }
    .maker {
      font-family: 'Arial Black', 'Helvetica Neue', Impact, Arial, sans-serif;
      font-weight: 900;
      font-size: 20px;
      letter-spacing: 0.02em;
      color: var(--red);
      text-shadow: 2px 2px 0 #1c100a;
    }

    /* ---------- Animations ---------- */
    @keyframes lamp {
      0%, 100% { filter: brightness(1); }
      50% { filter: brightness(1.25); }
    }
    @media (prefers-reduced-motion: reduce) {
      *, *::before, *::after { animation: none !important; transition: none !important; }
    }

    /* Desktop Layout */
    @media (min-width: 1024px) {
      body {
        padding: 15px;
      }
      .section {
        padding: 24px 20px 20px;
        width: 100%;
      }
      .sections-container {
        flex-direction: row;
        justify-content: center;
        align-items: flex-start;
        gap: 50px;
        padding: 0 20px;
      }
      .section-column {
        flex: 0 1 450px;
        gap: 20px;
      }
      .section {
        width: 100%;
        max-width: 450px;
        margin: 0;
      }
    }
  </style>
</head>
<body>
  <div class="hero">
    <h2 class="logo"><small><i>&#9733;</i> СКРИЧ <i>&#9733;</i></small><span>SCREECH</span></h2>

    <div class="stage" aria-hidden="true">
      <div class="vtext">ПЕРСОНАЛЬНЫЙ РОБОТ</div>
      <div class="bot">
        <div class="head">
          <div class="antenna"></div>
          <div class="visor"><div class="lens"></div><div class="lens"></div></div>
        </div>
        <div class="neck"></div>
      </div>
    </div>

    <div class="command-queue" id="queueStatus">Command Queue: 0/3</div>
  </div>
  <div class="stripes"></div>

  <div class="sections-container">
    <div class="section-column">
      <!-- Movement Control Section -->
      <div class="section">
        <div class="section-title">Movement Control <small>ДВИЖЕНИЕ</small></div>
        <div class="dpad-container">
          <div class="dpad">
            <div class="spacer"></div>
            <button onmousedown="move('forward')" onmouseup="stop()" ontouchstart="move('forward')" ontouchend="stop()">&#9650;</button>
            <div class="spacer"></div>

            <button onmousedown="move('left')" onmouseup="stop()" ontouchstart="move('left')" ontouchend="stop()">&#9664;&#65038;</button>
            <button onmousedown="move('backward')" onmouseup="stop()" ontouchstart="move('backward')" ontouchend="stop()">&#9660;</button>
            <button onmousedown="move('right')" onmouseup="stop()" ontouchstart="move('right')" ontouchend="stop()">&#9654;&#65038;</button>
          </div>
          <button class="btn-stop-all" onclick="stop()">STOP ALL <small>СТОП</small></button>
        </div>
      </div>

      <!-- Poses & Animations Section -->
      <div class="section">
        <div class="section-title">Poses &amp; Animations <small>ПОЗЫ</small></div>
        <div class="grid">
          <button class="btn-pose" onclick="pose('rest')">Rest</button>
          <button class="btn-pose" onclick="pose('stand')">Stand</button>
          <button class="btn-pose" onclick="pose('wave')">Wave</button>
          <button class="btn-pose" onclick="pose('dance')">Dance</button>
          <button class="btn-pose" onclick="pose('swim')">Swim</button>
          <button class="btn-pose" onclick="pose('point')">Point</button>
          <button class="btn-pose" onclick="pose('pushup')">Pushup</button>
          <button class="btn-pose" onclick="pose('bow')">Bow</button>
          <button class="btn-pose" onclick="pose('cute')">Cute</button>
          <button class="btn-pose" onclick="pose('freaky')">Freaky</button>
          <button class="btn-pose" onclick="pose('worm')">Worm</button>
          <button class="btn-pose" onclick="pose('shake')">Shake</button>
          <button class="btn-pose" onclick="pose('shrug')">Shrug</button>
          <button class="btn-pose" onclick="pose('dead')">Dead</button>
          <button class="btn-pose" onclick="pose('crab')">Crab</button>
        </div>
      </div>
    </div>

    <div class="section-column">
      <!-- Settings & Status Section -->
      <div class="section">
        <div class="section-title">System <small>СИСТЕМА</small></div>
        <button class="btn-settings" onclick="openSettings()">SETTINGS<small>НАСТРОЙКИ</small></button>
        <div style="margin-top: 16px;">
          <div id="gamepadStatus" class="gamepad-status">Gamepad disconnected</div>
        </div>
      </div>
    </div>
  </div>

  <footer>
    <div class="series">SCREECH SERIES<span>ПЕРСОНАЛЬНЫЙ&middot;РОБОТ</span></div>
    <div class="maker"><i>&#9733;</i> elya_screech</div>
  </footer>

  <div id="settingsPanel" class="settings-panel">
    <div class="settings-content">
      <h3>Settings<small>НАСТРОЙКИ</small></h3>

      <div class="settings-section">
        <h4>Animation Parameters</h4>
        <label>Frame Delay (ms):</label>
        <input type="number" id="frameDelay" min="1" max="1000" step="1">
        <label>Walk Cycles:</label>
        <input type="number" id="walkCycles" min="1" max="200" step="1">
      </div>

      <div class="settings-section">
        <h4>Motor Settings</h4>
        <label>Motor Current Delay (ms):</label>
        <input type="number" id="motorCurrentDelay" min="0" max="500" step="1">
        <label>Motor Speed:</label>
        <select id="motorSpeed">
          <option value="slow">Slow</option>
          <option value="medium" selected>Medium</option>
          <option value="fast">Fast</option>
        </select>
      </div>

      <div class="settings-section">
        <h4>Theme</h4>
        <label>Accent Color:</label>
        <select id="themeColor">
          <option value="#00aaff">Electric Blue (Default)</option>
          <option value="#ff6a2b">Flame Orange</option>
          <option value="#66d9ef">Cyan</option>
          <option value="#a8dadc">Light Blue</option>
          <option value="#2ecc71">Green</option>
          <option value="#e74c3c">Red</option>
          <option value="#9b59b6">Purple</option>
          <option value="#f39c12">Yellow</option>
          <option value="#e91e63">Pink</option>
          <option value="custom">Custom</option>
        </select>
        <input type="color" id="customColor" value="#00aaff" style="margin-top: 10px; display: none;">
      </div>

      <div class="settings-section">
        <h4>WiFi / Network</h4>
        <div id="wifiStatus" style="font-size:13px; color:#dcdcc4; margin-bottom:10px;">Checking&hellip;</div>
        <label>Network:</label>
        <select id="wifiSsid">
          <option value="">&mdash; scan for networks &mdash;</option>
        </select>
        <label style="margin-top:12px;">
          <input type="checkbox" id="wifiManual" style="width:auto; margin-right:8px;" onchange="toggleManualSsid()">
          Enter name manually (hidden network)
        </label>
        <input type="text" id="wifiSsidManual" placeholder="Network name" style="display:none;">
        <label>Password:</label>
        <input type="password" id="wifiPass" placeholder="WiFi password">
        <button class="btn-settings" style="width:100%; margin-top:12px;" onclick="scanWifi()">Scan for Networks</button>
        <button class="btn-settings" style="width:100%; margin-top:8px;" onclick="connectWifi()">Connect</button>
        <div id="wifiResult" style="font-size:13px; margin-top:10px; min-height:18px;"></div>
      </div>

      <button class="btn-settings" style="width: 100%; margin-top: 20px;" onclick="openMotorControl()">Manual Motor Control</button>

      <button class="btn-save" onclick="saveSettings()">Save Settings</button>
      <button class="btn-close" onclick="closeSettings()">Close</button>
    </div>
  </div>

  <div id="motorControlPanel" class="settings-panel">
    <div class="settings-content">
      <h3>Manual Motor Control</h3>
      <div class="lock-indicator" id="lockIndicator">Locked during animations</div>

      <div class="settings-section">
        <div class="motor-controls">
          <div class="motor-slider">
            <label><span>S0 R1</span><span id="m1val">90&deg;</span></label>
            <input type="range" id="motor1" min="0" max="180" value="90" oninput="updateMotor(1, this.value)">
          </div>
          <div class="motor-slider">
            <label><span>S1 R2</span><span id="m2val">90&deg;</span></label>
            <input type="range" id="motor2" min="0" max="180" value="90" oninput="updateMotor(2, this.value)">
          </div>
          <div class="motor-slider">
            <label><span>S2 L1</span><span id="m3val">90&deg;</span></label>
            <input type="range" id="motor3" min="0" max="180" value="90" oninput="updateMotor(3, this.value)">
          </div>
          <div class="motor-slider">
            <label><span>S3 L2</span><span id="m4val">90&deg;</span></label>
            <input type="range" id="motor4" min="0" max="180" value="90" oninput="updateMotor(4, this.value)">
          </div>
          <div class="motor-slider">
            <label><span>S4 R4</span><span id="m5val">90&deg;</span></label>
            <input type="range" id="motor5" min="0" max="180" value="90" oninput="updateMotor(5, this.value)">
          </div>
          <div class="motor-slider">
            <label><span>S5 R3</span><span id="m6val">90&deg;</span></label>
            <input type="range" id="motor6" min="0" max="180" value="90" oninput="updateMotor(6, this.value)">
          </div>
          <div class="motor-slider">
            <label><span>S6 L3</span><span id="m7val">90&deg;</span></label>
            <input type="range" id="motor7" min="0" max="180" value="90" oninput="updateMotor(7, this.value)">
          </div>
          <div class="motor-slider">
            <label><span>S7 L4</span><span id="m8val">90&deg;</span></label>
            <input type="range" id="motor8" min="0" max="180" value="90" oninput="updateMotor(8, this.value)">
          </div>
        </div>
      </div>

      <button class="btn-close" onclick="closeMotorControl()">Close</button>
    </div>
  </div>

<script>
// Command queue management - max 3 commands
let commandQueue = 0;
const MAX_COMMANDS = 3;
let motorsLocked = false;

// Load theme on page load
document.addEventListener('DOMContentLoaded', () => {
  loadTheme();
});


function loadTheme() {
  const savedColor = localStorage.getItem('themeColor');
  if (savedColor) {
    applyTheme(savedColor);
  }
}

function applyTheme(color) {
  const root = document.documentElement;
  root.style.setProperty('--content-color', color);
  
  // Calculate darker shades
  const rgb = hexToRgb(color);
  if (rgb) {
    const dark = `rgb(${Math.max(0, rgb.r - 20)}, ${Math.max(0, rgb.g - 20)}, ${Math.max(0, rgb.b - 20)})`;
    const darker = `rgb(${Math.max(0, rgb.r - 40)}, ${Math.max(0, rgb.g - 40)}, ${Math.max(0, rgb.b - 40)})`;
    const glow = `rgba(${rgb.r}, ${rgb.g}, ${rgb.b}, 0.3)`;
    
    root.style.setProperty('--content-color-dark', dark);
    root.style.setProperty('--content-color-darker', darker);
    root.style.setProperty('--content-color-glow', glow);
  }
}

function hexToRgb(hex) {
  const result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
  return result ? {
    r: parseInt(result[1], 16),
    g: parseInt(result[2], 16),
    b: parseInt(result[3], 16)
  } : null;
}

function updateQueueStatus() {
  const queueEl = document.getElementById('queueStatus');
  queueEl.textContent = `Command Queue: ${commandQueue}/${MAX_COMMANDS}`;
  if (commandQueue >= MAX_COMMANDS) {
    queueEl.classList.add('full');
  } else {
    queueEl.classList.remove('full');
  }
}

function canSendCommand() {
  return commandQueue < MAX_COMMANDS;
}

function incrementQueue() {
  commandQueue++;
  updateQueueStatus();
  setTimeout(() => {
    if (commandQueue > 0) {
      commandQueue--;
    }
    updateQueueStatus();
  }, 1000);
}

function lockMotors(duration = 3000) {
  motorsLocked = true;
  document.getElementById('lockIndicator').classList.add('active');
  for (let i = 1; i <= 8; i++) {
    const slider = document.getElementById('motor' + i);
    if (slider) slider.disabled = true;
  }
  setTimeout(() => {
    motorsLocked = false;
    document.getElementById('lockIndicator').classList.remove('active');
    for (let i = 1; i <= 8; i++) {
      const slider = document.getElementById('motor' + i);
      if (slider) slider.disabled = false;
    }
  }, duration);
}

function move(dir) { 
  if (!canSendCommand()) return;
  incrementQueue();
  fetch('/cmd?go=' + dir).catch(console.log); 
}

function stop() { 
  commandQueue = 0;
  updateQueueStatus();
  fetch('/cmd?stop=1').catch(console.log); 
}

function pose(name) { 
  if (!canSendCommand()) return;
  incrementQueue();
  lockMotors(3000);
  fetch('/cmd?pose=' + name).catch(console.log); 
}

function updateMotor(motorNum, value) {
  if (motorsLocked) return;
  document.getElementById('m' + motorNum + 'val').textContent = value + '\u00B0';
  if (!canSendCommand()) return;
  incrementQueue();
  fetch('/cmd?motor=' + motorNum + '&value=' + value).catch(console.log);
}

function openSettings() {
  loadWifiStatus();
  fetch('/getSettings').then(r => r.json()).then(data => {
    document.getElementById('frameDelay').value = data.frameDelay || 50;
    document.getElementById('walkCycles').value = data.walkCycles || 75;
    document.getElementById('motorCurrentDelay').value = data.motorCurrentDelay || 20;
    document.getElementById('motorSpeed').value = data.motorSpeed || 'medium';
    
    // Load theme settings
    const savedColor = localStorage.getItem('themeColor') || '#00aaff';
    const colorSelect = document.getElementById('themeColor');
    const customColorInput = document.getElementById('customColor');
    
    // Check if saved color matches a preset
    let matchFound = false;
    for (let option of colorSelect.options) {
      if (option.value === savedColor) {
        colorSelect.value = savedColor;
        matchFound = true;
        break;
      }
    }
    
    if (!matchFound) {
      colorSelect.value = 'custom';
      customColorInput.value = savedColor;
      customColorInput.style.display = 'block';
    }
    
    document.getElementById('settingsPanel').style.display = 'block';
  }).catch(() => {
    // Fallback if settings endpoint doesn't exist yet
    document.getElementById('frameDelay').value = 50;
    document.getElementById('walkCycles').value = 75;
    document.getElementById('motorCurrentDelay').value = 20;
    
    const savedColor = localStorage.getItem('themeColor') || '#00aaff';
    document.getElementById('themeColor').value = savedColor;
    
    document.getElementById('settingsPanel').style.display = 'block';
  });
  
  // Add event listener for theme color change
  document.getElementById('themeColor').addEventListener('change', function() {
    const customColorInput = document.getElementById('customColor');
    if (this.value === 'custom') {
      customColorInput.style.display = 'block';
    } else {
      customColorInput.style.display = 'none';
      applyTheme(this.value);
    }
  });
  
  document.getElementById('customColor').addEventListener('input', function() {
    applyTheme(this.value);
  });
}

function closeSettings() { 
  document.getElementById('settingsPanel').style.display = 'none'; 
}

function openMotorControl() {
  document.getElementById('motorControlPanel').style.display = 'block';
}

function closeMotorControl() {
  document.getElementById('motorControlPanel').style.display = 'none';
}

function saveSettings() {
  const fd = document.getElementById('frameDelay').value;
  const wc = document.getElementById('walkCycles').value;
  const mcd = document.getElementById('motorCurrentDelay').value;
  const ms = document.getElementById('motorSpeed').value;
  
  // Save theme color
  const colorSelect = document.getElementById('themeColor');
  const customColorInput = document.getElementById('customColor');
  const themeColor = colorSelect.value === 'custom' ? customColorInput.value : colorSelect.value;
  localStorage.setItem('themeColor', themeColor);
  applyTheme(themeColor);
  
  fetch(`/setSettings?frameDelay=${fd}&walkCycles=${wc}&motorCurrentDelay=${mcd}&motorSpeed=${ms}`)
    .then(() => closeSettings())
    .catch(() => closeSettings());
}

function toggleManualSsid() {
  const manual = document.getElementById('wifiManual').checked;
  document.getElementById('wifiSsidManual').style.display = manual ? 'block' : 'none';
  document.getElementById('wifiSsid').style.display = manual ? 'none' : 'block';
}

// Builds "http://<host> (<ip>)" as safe DOM nodes (no innerHTML — SSIDs/values
// are untrusted and could carry markup).
function appendRobotLink(parent, host, ip) {
  const a = document.createElement('a');
  a.href = 'http://' + host;
  a.textContent = 'http://' + host;
  a.style.color = 'var(--content-color)';
  parent.appendChild(a);
  parent.appendChild(document.createTextNode(' (' + ip + ')'));
}

// Appends "http://<host> (<ip>)" — or just the raw IP when the robot reports
// its mDNS responder didn't start — as safe DOM nodes.
function appendRobotAddress(parent, d) {
  if (d.mdns) {
    appendRobotLink(parent, d.host, d.ip);
  } else {
    parent.appendChild(document.createTextNode('http://' + d.ip));
  }
}

function loadWifiStatus() {
  fetch('/api/wifi/status').then(r => r.json()).then(d => {
    const el = document.getElementById('wifiStatus');
    el.textContent = '';
    if (d.connected) {
      el.appendChild(document.createTextNode('Connected to '));
      const b = document.createElement('b');
      b.textContent = d.ssid;            // textContent: SSID is untrusted
      el.appendChild(b);
      el.appendChild(document.createTextNode(' — '));
      appendRobotAddress(el, d);
    } else if (d.connecting) {
      el.textContent = 'Connecting…';
    } else {
      el.textContent = 'Not connected (Access Point mode only).';
    }
  }).catch(() => {
    document.getElementById('wifiStatus').textContent = 'Status unavailable.';
  });
}

// The scan runs asynchronously on the robot: /api/wifi/scan answers
// {"scanning":true} until results are ready, so poll about once a second.
function scanWifi() {
  document.getElementById('wifiResult').textContent = 'Scanning…';
  pollWifiScan(0);
}

function pollWifiScan(attempt) {
  const sel = document.getElementById('wifiSsid');
  const result = document.getElementById('wifiResult');
  fetch('/api/wifi/scan').then(r => r.json()).then(data => {
    if (!Array.isArray(data)) {
      if (attempt < 20) setTimeout(() => pollWifiScan(attempt + 1), 1000);
      else result.textContent = 'Scan timed out.';
      return;
    }
    // Map (not {}): SSIDs are untrusted and may collide with
    // Object.prototype keys ('constructor', '__proto__', ...).
    const best = new Map();
    data.forEach(n => {
      if (!n.ssid) return;
      if (!best.has(n.ssid) || n.rssi > best.get(n.ssid).rssi) best.set(n.ssid, n);
    });
    const nets = Array.from(best.values()).sort((a, b) => b.rssi - a.rssi);
    sel.textContent = '';
    const ph = document.createElement('option');
    ph.value = '';
    ph.textContent = '— select network —';
    sel.appendChild(ph);
    nets.forEach(n => {
      const pct = Math.min(100, Math.max(0, 2 * (n.rssi + 100)));
      const opt = document.createElement('option');
      opt.value = n.ssid;
      opt.textContent = n.ssid + ' (' + pct + '%' + (n.secure ? ' secured' : '') + ')';
      sel.appendChild(opt);
    });
    result.textContent = nets.length ? (nets.length + ' networks found.') : 'No networks found.';
  }).catch(() => {
    // Transient AP hiccup — retry on the same schedule.
    if (attempt < 20) setTimeout(() => pollWifiScan(attempt + 1), 1000);
    else result.textContent = 'Scan failed.';
  });
}

// POST starts the attempt; the robot connects in the background (its access
// point may briefly drop while it changes WiFi channel), so the outcome is
// read by polling /api/wifi/status — which also survives that brief drop.
function connectWifi() {
  const manual = document.getElementById('wifiManual').checked;
  const ssid = manual ? document.getElementById('wifiSsidManual').value.trim()
                      : document.getElementById('wifiSsid').value;
  const pass = document.getElementById('wifiPass').value;
  const result = document.getElementById('wifiResult');
  if (!ssid) { result.textContent = 'Please select or enter a network.'; return; }
  result.textContent = 'Connecting to ' + ssid + '… (can take ~15s)';
  const body = 'ssid=' + encodeURIComponent(ssid) + '&password=' + encodeURIComponent(pass);
  fetch('/api/wifi/connect', {
    method: 'POST',
    headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
    body: body
  }).then(r => r.json()).then(d => {
    if (d.success) {
      setTimeout(() => pollWifiConnect(0), 1500);
    } else {
      result.textContent = 'Failed: ' + (d.error || 'could not start connection') + '.';
    }
  }).catch(() => {
    // The request may have been cut off mid-flight; the attempt is most
    // likely still running on the robot, so poll for the outcome anyway.
    setTimeout(() => pollWifiConnect(0), 1500);
  });
}

function pollWifiConnect(attempt) {
  const result = document.getElementById('wifiResult');
  fetch('/api/wifi/status').then(r => r.json()).then(d => {
    if (d.connected) {
      result.textContent = 'Connected! Reach the robot at ';
      appendRobotAddress(result, d);
      loadWifiStatus();
      return;
    }
    if (d.connecting) {
      if (attempt < 30) setTimeout(() => pollWifiConnect(attempt + 1), 1000);
      else result.textContent = 'Still trying… reopen Settings to check the status.';
      return;
    }
    result.textContent = 'Failed: ' + (d.lastError || 'could not connect') +
      '. Check the password and that it is a 2.4GHz network.';
  }).catch(() => {
    // The phone may be rejoining the robot's AP after a channel change.
    if (attempt < 30) setTimeout(() => pollWifiConnect(attempt + 1), 1000);
    else result.textContent = 'Lost contact with the robot. Rejoin its WiFi network and reopen Settings.';
  });
}

let activeGamepadIndex = null;
let gamepadPollId = null;
let lastButtonStates = [];
let lastAxisDir = { x: 0, y: 0 };
const axisThreshold = 0.5;
const pollIntervalMs = 80;

const buttonBindings = {
  0: () => pose('stand'),   // A / Cross
  1: () => pose('wave'),    // B / Circle
  2: () => pose('dance'),   // X / Square
  3: () => pose('swim'),    // Y / Triangle
  4: () => pose('point'),   // LB / L1
  5: () => pose('pushup'),  // RB / R1
  6: () => pose('bow'),     // LT / L2
  7: () => pose('shake'),   // RT / R2
  8: () => stop(),          // Back / Share
  9: () => pose('rest'),    // Start / Options
  10: () => pose('cute'),   // L3
  11: () => pose('freaky'), // R3
  12: () => move('forward'),// D-pad up
  13: () => move('backward'),// D-pad down
  14: () => move('left'),   // D-pad left
  15: () => move('right'),  // D-pad right
  16: () => stop(),         // Home / PS
  17: () => pose('worm')    // Touchpad / extra
};

const buttonReleaseStop = new Set([12, 13, 14, 15]);

function updateGamepadStatus(connected) {
  const status = document.getElementById('gamepadStatus');
  if (!status) return;
  if (connected) {
    status.textContent = 'Gamepad connected';
    status.classList.add('connected');
  } else {
    status.textContent = 'Gamepad disconnected';
    status.classList.remove('connected');
  }
}

function handleButtonChange(index, pressed) {
  if (pressed) {
    const action = buttonBindings[index];
    if (action) action();
  } else if (buttonReleaseStop.has(index)) {
    stop();
  }
}

function getAxisDirection(x, y) {
  if (Math.abs(x) < axisThreshold && Math.abs(y) < axisThreshold) return { x: 0, y: 0 };
  if (Math.abs(x) > Math.abs(y)) {
    return { x: x > 0 ? 1 : -1, y: 0 };
  }
  return { x: 0, y: y > 0 ? 1 : -1 };
}

function applyAxisDirection(dir) {
  if (dir.x === 1) move('right');
  else if (dir.x === -1) move('left');
  else if (dir.y === 1) move('backward');
  else if (dir.y === -1) move('forward');
  else stop();
}

function pollGamepad() {
  const pads = navigator.getGamepads ? navigator.getGamepads() : [];
  const pad = pads && activeGamepadIndex !== null ? pads[activeGamepadIndex] : null;
  if (!pad) {
    updateGamepadStatus(false);
    return;
  }
  updateGamepadStatus(true);

  if (!lastButtonStates.length) {
    lastButtonStates = pad.buttons.map(b => !!b.pressed);
  }
  pad.buttons.forEach((btn, i) => {
    const pressed = !!btn.pressed;
    if (pressed !== lastButtonStates[i]) {
      handleButtonChange(i, pressed);
      lastButtonStates[i] = pressed;
    }
  });

  const x = pad.axes[0] || 0;
  const y = pad.axes[1] || 0;
  const dir = getAxisDirection(x, y);
  if (dir.x !== lastAxisDir.x || dir.y !== lastAxisDir.y) {
    applyAxisDirection(dir);
    lastAxisDir = dir;
  }
}

window.addEventListener('gamepadconnected', (e) => {
  activeGamepadIndex = e.gamepad.index;
  lastButtonStates = [];
  lastAxisDir = { x: 0, y: 0 };
  updateGamepadStatus(true);
  if (!gamepadPollId) {
    gamepadPollId = setInterval(pollGamepad, pollIntervalMs);
  }
});

window.addEventListener('gamepaddisconnected', (e) => {
  if (activeGamepadIndex === e.gamepad.index) {
    activeGamepadIndex = null;
    lastButtonStates = [];
    lastAxisDir = { x: 0, y: 0 };
    updateGamepadStatus(false);
  }
});

if (navigator.getGamepads) {
  setInterval(() => {
    if (activeGamepadIndex !== null) return;
    const pads = navigator.getGamepads();
    if (!pads) return;
    for (let i = 0; i < pads.length; i++) {
      if (pads[i]) {
        activeGamepadIndex = pads[i].index;
        updateGamepadStatus(true);
        if (!gamepadPollId) {
          gamepadPollId = setInterval(pollGamepad, pollIntervalMs);
        }
        break;
      }
    }
  }, 1000);
}
</script>
</body>
</html>
)rawliteral";
