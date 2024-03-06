let initial_sessions = [];
let initial_totals = [];

if (
  typeof window !== "undefined" &&
  localStorage.hasOwnProperty("sessions")
) {
  initial_sessions = JSON.parse(localStorage.getItem("sessions"));
}

if (
  typeof window !== "undefined" &&
  localStorage.hasOwnProperty("totals")
) {
  initial_totals = JSON.parse(localStorage.getItem("totals"));
}

let sessions = $state(initial_sessions);
let sessionTotals = $state(initial_totals);

import { fetchSessions } from "../http-actions/sessionManagement";

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
