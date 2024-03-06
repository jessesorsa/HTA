<script>
    import SessionList from "./SessionList.svelte";
    import Stats from "./Stats.svelte";
    import { onMount } from "svelte";
    import { initStores, sessionStore } from "../stores/store.svelte";
    const localSessionStore = sessionStore();

    onMount(async () => {
        await initStores();
    });

    const updateSessions = async () => {
        await initStores();
        localSessionStore.updateSessionTotals();
    };
</script>

<div class="hero p-10 flex flex-col items-start bg-light-grey-color">
    <div class="max-w-md">
        <h1 class="text-5xl font-bold">Welcome user!</h1>
        <div class="flex flex-row items-center">
            <p class="py-5">Update your sessions list</p>
            <button
                class="btn btn-outline ml-3 btn-sm"
                on:click={updateSessions}
                ><svg
                    xmlns="http://www.w3.org/2000/svg"
                    class="h-3 w-3"
                    fill="none"
                    viewBox="0 0 24 24"
                    stroke="currentColor"
                    stroke-linecap="round"
                    stroke-linejoin="round"
                    stroke-width="2"
                    ><path
                        d="M2.5 2v6h6M2.66 15.57a10 10 0 1 0 .57-8.38"
                    /></svg
                ></button
            >
        </div>
    </div>
    <div class="flex flex-row">
        <div class="mr-10">
            <SessionList />
        </div>
        <div class="flex flex-col items-center">
            <Stats />
        </div>
    </div>
</div>
