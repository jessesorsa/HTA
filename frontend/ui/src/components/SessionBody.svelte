<script>
    import { sessionStore } from "../stores/store.svelte";
    import * as Api from "../http-actions/sessionManagement";
    const localSessionStore = sessionStore();

    const deleteSession = async (session) => {
        await Api.deleteSession(session);
        localSessionStore.deleteSession(session);
        localSessionStore.updateSessionTotals();
    };
</script>

{#each localSessionStore.listSessions as session}
    <tr class="hover">
        {#each session as item}
            <th>{item}</th>
        {/each}
        <td
            ><button
                class="btn btn-ghost btn-circle"
                on:click={() => deleteSession(session)}
                ><svg
                    xmlns="http://www.w3.org/2000/svg"
                    class="h-5 w-5"
                    fill="none"
                    viewBox="0 0 24 24"
                    stroke="currentColor"
                    stroke-linecap="round"
                    stroke-linejoin="round"
                    stroke-width="2"
                    ><polyline points="3 6 5 6 21 6"></polyline><path
                        d="M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6m3 0V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2"
                    ></path><line x1="10" y1="11" x2="10" y2="17"></line><line
                        x1="14"
                        y1="11"
                        x2="14"
                        y2="17"
                    ></line></svg
                ></button
            ></td
        >
    </tr>
{/each}
