<script>
    import { sessionStore } from "../stores/store.svelte";
    const localSessionStore = sessionStore();
</script>

<div class="stats stats-vertical shadow">
    <div class="stat py-3">
        <div class="stat-desc font-bold text-xs">Total statistics</div>
    </div>
    <div class="stat">
        <div class="stat-title">Distance</div>
        {#if localSessionStore.listSessionTotals.length > 0}
            <div class="stat-value">
                {localSessionStore.listSessionTotals[0]} km
            </div>{/if}
    </div>
    <div class="stat">
        <div class="stat-title">Step count</div>
        {#if localSessionStore.listSessionTotals.length > 0}
            <div class="stat-value">
                {localSessionStore.listSessionTotals[1]}
            </div>{/if}
    </div>

    <div class="stat">
        <div class="stat-title">Calories burned</div>
        {#if localSessionStore.listSessionTotals.length > 0}
            <div class="stat-value">
                {localSessionStore.listSessionTotals[2]} kcal
            </div>{/if}
    </div>
</div>
