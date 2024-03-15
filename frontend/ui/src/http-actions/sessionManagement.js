const fetchSessions = async () => {
    const res = await fetch("http://192.168.114.143:5000/sessions");
    const data = await res.json();
    return data;
};

const deleteSession = async (session) => {
    const res = await fetch(`http://192.168.114.143:5000/sessions/${session[0]}/delete`);
    console.log(res);
    const data = await res.json();
    return data;
};

export { fetchSessions, deleteSession };