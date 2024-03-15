MET_HIKING = 6
KCAL_PER_STEP = 0.04

class HikeSession:
    id = 0
    km = 0
    steps = 0
    kcal = -1
    coords = []
    start_time = 0
    duration = 0

    # represents a computationally intensive calculation done by lazy execution.
    def calc_kcal(self):
        self.kcal = MET_HIKING * KCAL_PER_STEP * self.steps

    def __repr__(self):
        return f"HikeSession{{{self.id}, {self.km}(km), {self.steps}(steps), {self.kcal:.2f}(kcal), {self.start_time}(start_time), {self.duration}(duration)}}"

def to_list(s: HikeSession) -> list:
    return [s.id, s.km, s.steps, s.kcal, s.start_time, s.duration]

def from_list(l: list) -> HikeSession:
    s = HikeSession()
    s.id = l[0]
    s.km = l[1]
    s.steps = l[2]
    s.kcal = l[3]
    s.start_time = l[4]
    s.duration = l[5]
    return s