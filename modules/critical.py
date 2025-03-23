import tildagon_helpers

class Critical:
    def __enter__(self):
        print("Entering critical section")
        tildagon_helpers.enter_critical_section()

    def __exit__(self, exc_type, exc_value, exc_tb):
        tildagon_helpers.leave_critical_section()
        print("Leaving critical section")