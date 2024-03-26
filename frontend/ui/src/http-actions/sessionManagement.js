// fetching all sessions from the database server
const fetchSessions = async () => {
    const res = await fetch("http://192.168.43.207:5000/sessions");
    const data = await res.json();
    return data;
};

// sending a fetch request to delete the session with id = session[0]
const deleteSession = async (session) => {
    const res = await fetch(`http://192.168.43.207:5000/sessions/${session[0]}/delete`);
    console.log(res);
    const data = await res.json();
    return data;
};

export { fetchSessions, deleteSession };