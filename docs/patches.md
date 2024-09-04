# Patches

There are several patches enabled by default, however some advanced patches must be manually configured in a `patches.ini` file.

```ini
[Patches]
HeapSize = 512M
MaxShadowMap = 8192
```

## Heap size

By default the game reserves 256 megabytes of heap memory to use, this patch allows you to increase this value.

> [!WARNING]  
> The operating system might refuse to reserve a large amount of memory when increasing this value.

## Shadow map size

This patch allows you to increase the maximum size of shadow maps.
