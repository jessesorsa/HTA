// inital arrays
let initial_sessions = [];
let initial_totals = [];

// checking if the browser locaStorage has sessions stored
// if they are found, they are the initial sessions
if (
  typeof window !== "undefined" &&
  localStorage.hasOwnProperty("sessions")
) {
  initial_sessions = JSON.parse(localStorage.getItem("sessions"));
}

// checking if the browser locaStorage has totals stored
// if they are found, they are the set to be initial totals
if (
  typeof window !== "undefined" &&
  localStorage.hasOwnProperty("totals")
) {
  initial_totals = JSON.parse(localStorage.getItem("totals"));
}

// determining svelte state variables and giving them the initial values
let sessions = $state(initial_sessions);
let sessionTotals = $state(initial_totals);

import { fetchSessions } from "../http-actions/sessionManagement";

// initStores is responsible for initializing the localStorage with values 
// that are fetched from the database server
const initStores = async () => {
  const sessionList = await fetchSessions();
  sessions = sessionList;
  localStorage.setItem("sessions", JSON.stringify(sessionList));

  let distance = 0;
  let step = 0;
  let kcal = 0;
  sessionList.forEach((session) => {
    distance += session[1];
    step += session[2];
    kcal += session[3];
  });
  const totals = [distance, step, kcal];
  sessionTotals = totals;
  localStorage.setItem("totals", JSON.stringify(totals))

};


// sessionStore is used to change and list values in the localStorage and
// svelte state variables
const sessionStore = () => {
  return {
    get listSessions() {
      return sessions;
    },
    get listSessionTotals() {
      return sessionTotals;
    },
    deleteSession: (session) => {
      sessions.forEach((item, index) => {
        if (item === session) {
          sessions.splice(index, 1);
        }
      });
      localStorage.setItem("sessions", JSON.stringify(sessions));
    },
    updateSessionTotals: () => {
      let distance = 0;
      let step = 0;
      let kcal = 0;
      sessions.forEach((session) => {
        distance += session[1];
        step += session[2];
        kcal += session[3];
      });
      const totals = [distance, step, kcal];
      sessionTotals = totals;
      localStorage.setItem("totals", JSON.stringify(sessions));
    }
  };
};

export { sessionStore, initStores };
