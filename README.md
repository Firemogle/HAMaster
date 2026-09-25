# HAMaster

Home Assistant configuration for the Carriger home. This repository contains the YAML configuration, automations, scripts, scenes, ESPHome configurations, dashboards, and supporting files used to operate the home.

## Repository structure

| Path | Purpose |
| --- | --- |
| `configuration.yaml` | Main Home Assistant configuration and includes |
| `packages/` | Primary modular Home Assistant configuration |
| `automations/` | Standalone automation configuration |
| `scripts/` | Reusable Home Assistant scripts |
| `scenes/` | Home Assistant scenes |
| `people/` | Person definitions and related configuration |
| `esphome/` | ESPHome device configurations and packages |
| `frigate/` | Frigate configuration |
| `cameras/` | Camera-related configuration |
| `themes/` | Home Assistant themes |
| `zones/` | Home Assistant zones |
| `ui-lovelace.yaml` | Lovelace dashboard configuration |
| `groups.yaml` | Home Assistant groups |
| `notify.yaml` | Notification configuration |

## Configuration organization

Most current home logic is organized into files under `packages/`, with separate files for areas and functions such as:

- HVAC and humidity control
- Lighting and time-of-day behavior
- Occupancy and household status
- Kitchen, bathrooms, bedrooms, garage, basement, office, and exterior areas
- Holidays and seasonal lighting
- Energy and DTE-related calculations
- Irrigation and backyard systems
- Network and device-related configuration
- Sports and notification-related automations

The package-based organization keeps related entities, automations, scripts, and helpers together instead of placing the entire configuration in a small number of monolithic files.

## Startup behavior

Home Assistant startup is handled by the `ha_start` automation in `packages/system.yaml`.

The startup sequence:

1. Waits five minutes after Home Assistant starts.
2. Synchronizes calendar-driven system input booleans.
3. Sends the Home Assistant startup notification.

There is no longer a global `automation.turn_on` operation at startup. Automations that need to avoid acting during initialization are enabled normally and use the `binary_sensor.house_boot_complete` condition.

The current boot-complete sensor is defined in `packages/system.yaml`:

```yaml
- name: house_boot_complete
  state: "{{ states('sensor.uptime') | float(0) > 5 }}"
```

This sensor is used as a startup guard for selected system, time, and HVAC automations.

## Abode alarm synchronization

The configuration synchronizes the local alarm panel with the Abode alarm entity in both directions.

The synchronized states are:

- `disarmed`
- `armed_away`
- `armed_home`

Each synchronization automation has two safeguards:

1. `binary_sensor.house_boot_complete` must be `on`.
2. The receiving alarm panel must not already be in the requested state.

The second check prevents unnecessary service calls and protects against a two-way synchronization loop where changing one alarm entity causes the other to change it back.

For example:

```yaml
condition:
  - condition: state
    entity_id: binary_sensor.house_boot_complete
    state: 'on'
  - condition: state
    entity_id: alarm_control_panel.alarm_panel
    state_not: 'armed_away'
```

## Secrets

Sensitive values such as alarm codes are stored using Home Assistant's `!secret` mechanism rather than committed directly into the repository.

Do not commit passwords, API tokens, private keys, alarm codes, or other credentials to this repository.

## ESPHome

The `esphome/` directory contains configurations for the home's ESPHome devices and shared ESPHome packages. Device configurations may depend on hardware-specific components, external components, and Home Assistant integrations.

Before deploying an ESPHome configuration, validate it against the installed ESPHome version and the target hardware.

## Making changes

When modifying the configuration:

1. Keep related logic in the appropriate package or functional directory.
2. Prefer native Home Assistant conditions and actions over unnecessary Jinja templating.
3. Use reusable scripts for operations performed by multiple automations.
4. Avoid global startup actions when a targeted startup condition is sufficient.
5. Protect bidirectional integrations from unnecessary state changes or synchronization loops.
6. Validate YAML and Home Assistant configuration before deploying changes.
7. Keep secrets outside the repository.

## Backup files

Temporary editor and backup files such as `.bak` and `.old` are not part of the maintained configuration and should not be committed.

## Notes

This is a personal Home Assistant configuration rather than a generic drop-in configuration. Entity IDs, device IDs, areas, secrets, integrations, and hardware references are specific to the installation.

